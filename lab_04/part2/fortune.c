// Фортунки

#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h>  
#include <linux/vmalloc.h>
#include <linux/proc_fs.h> 
#include <asm/uaccess.h>



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tsvetkov IU7-63B");
MODULE_DESCRIPTION("LAB_04 (Part 2): fortunes");



#define FORTUNE_DIRNAME "fortune_dir"
#define FORTUNE_FILENAME "fortune_file"
#define FORTUNE_SYMLINK "fortune_symlink"
#define FORTUNE_PATH FORTUNE_FILENAME

#define COOKIE_MAX_BUF_SIZE PAGE_SIZE // 4096 байт


static struct proc_dir_entry *fortuneDir;
static struct proc_dir_entry *fortuneFile;
static struct proc_dir_entry *fortuneSymlink;

// Хранилище для фортунок
static char *cookieBuffer = NULL;
char tmpBuffer[COOKIE_MAX_BUF_SIZE];

static int readInd = 0;
static int writeInd = 0;


static int openFortune(struct inode *spInode, struct file *spFile)
{
    printk(KERN_INFO "+ Fortune (open): %s called\n", __func__);

    return 0;
}


static int releaseFortune(struct inode *spInode, struct file *spFile)
{
    printk(KERN_INFO "+ Fortune (release): %s called\n", __func__);

    return 0;
}


// Запись в ядро
// 1 - указатель на структуру file
// 2 - буфер данныъ длдя чтения
// 3 - размер передаваемых данных
// 4 - смещение от начала данных файла для операции чтения
static ssize_t writeFortune(struct file *file, const char __user *buf, size_t len, loff_t *fPos)
{
    printk(KERN_INFO "+ Fortune (write): %s called\n", __func__);


    // Проверка достаточности места для фортунки
    if (len > COOKIE_MAX_BUF_SIZE - writeInd + 1)
    {
        printk(KERN_ERR "+ Fortune (write): Buffer is overflowed\n");
        return -ENOSPC;
    }

    // Копирование буфера из пространства пользователя
    // в пространоство ядра
    // 1 - место в пространстве ядра
    // 2 - место из пространства пользователя
    // 3 - кол-во байт
    if (copy_from_user(&cookieBuffer[writeInd], buf, len) != 0)
    {
        printk(KERN_ERR "+ Fortune (write): copy_from_user returned error\n");
        return -EFAULT;
    }

    writeInd += len;
    cookieBuffer[writeInd - 1] = '\n';

    cookieBuffer[writeInd] = '\0';
    writeInd += 1;

    // Кол-во символов, записанных в буфер (cookieBuffer)
    return len;
}


// Чтение из ядра
// 1 - указатель на структуру file
// 2 - буфер данных записи
// 3 - размер передаваемых данных
// 4 - смещение от начала данных файла для операции записи
static ssize_t readFortune(struct file *file, char __user *buf, size_t len, loff_t *fPos)
{   
    int readLen;

    printk(KERN_INFO "+ Fortune (read): %s called\n", __func__);

    if ((*fPos > 0) || (writeInd == 0))
    {
        // printk(KERN_ERR "+ Fortune (read): File error\n");
        return 0;
    }

    // Для зацикливания буффера
    // Если дошли до места, куда собираемся что-то писать
    if (readInd >= writeInd)
    {
        readInd = 0;
    }

    readLen = snprintf(tmpBuffer, COOKIE_MAX_BUF_SIZE, "%s\n", &cookieBuffer[readInd]);

    // Из пр-ва ядра в пр-во пользователя
    // 1 - адрес назначения в пр-ве пользователя
    // 2 - адрес источника в пр-ве ядра
    // 3 - кол-во байт для копирования
    if (copy_to_user(buf, tmpBuffer, readLen) != 0)
    {
        printk(KERN_ERR "+ Fortune (read): copy_to_user returned error\n");
        return -EFAULT;
    }

    readInd += readLen;
    *fPos += readLen;

    return readLen;
}



static const struct proc_ops fops =
{
    proc_read: readFortune,
    proc_write: writeFortune,
    proc_open: openFortune,
    proc_release: releaseFortune,
};


static void cleanUpFortune(void)
{
    printk(KERN_INFO "+ Fortune (cleanUp): %s called\n", __func__);

    if (fortuneSymlink != NULL)
    {
        remove_proc_entry(FORTUNE_SYMLINK, NULL);
    }

    if (fortuneFile != NULL)
    {
        remove_proc_entry(FORTUNE_FILENAME, NULL);
    }

    if (fortuneDir != NULL)
    {
        remove_proc_entry(FORTUNE_DIRNAME, NULL);
    }

    vfree(cookieBuffer);
}


static int __init initFortune(void)
{
    printk(KERN_INFO "+ Fortune (init): %s called\n", __func__);

    // Выделить виртуально непрерывный блок памяти
    if ((cookieBuffer = vmalloc(COOKIE_MAX_BUF_SIZE)) == NULL)
    {
        printk(KERN_ERR "+ Fortune (init): Allocate memory error\n");
        return -ENOMEM;
    }

    memset(cookieBuffer, 0, COOKIE_MAX_BUF_SIZE);

    // Указатель на proc_dir_entry (корневой каталог)
    // Имя самой директории
    if ((fortuneDir = proc_mkdir(FORTUNE_DIRNAME, NULL)) == NULL)
    {
        printk(KERN_ERR "+ Fortune (init): Create dir in proc error\n");
        cleanUpFortune();
        return -ENOMEM;
    }

    // Создание файла (= регистрация структуры)
    // Имя файла
    // Права доступа - 0666 (всем можно читать и писать)
    // Указатель на родительскую директорию - NULL (корневой каталог /proc)
    // Указатель на операции на файлах - fops
    if ((fortuneFile = proc_create(FORTUNE_FILENAME, 0666, NULL, &fops)) == NULL)
    {
        printk(KERN_ERR "+ Fortune (init): Create file in proc error\n");
        cleanUpFortune();
        return -ENOMEM;
    }

    
    // Создание символической ссылки
    // Имя ссылки
    // Указатель на корневой каталог /proc
    // Имя файла, на который указывает ссылка
    if ((fortuneSymlink = proc_symlink(FORTUNE_SYMLINK, NULL, FORTUNE_PATH)) == NULL)
    {
        printk(KERN_ERR "+ Fortune (init): Create symlink in proc error\n");
        cleanUpFortune();
        return -ENOMEM;
    }

    // Начальные значения
    readInd = 0;
    writeInd = 0;

    printk(KERN_INFO "+ Fortune (init): Module loadded with no errors\n");

    return 0;
}


static void __exit exitFortune(void)
{
    printk(KERN_INFO "+ Fortune (exit): %s called\n", __func__);
    cleanUpFortune();

    printk(KERN_INFO "+ Fortune (exit): Module removed");
}


module_init(initFortune);
module_exit(exitFortune);
