// seqfile

#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h>  
#include <linux/vmalloc.h>
#include <linux/proc_fs.h> 
#include <linux/seq_file.h>
#include <asm/uaccess.h>



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tsvetkov IU7-63B");
MODULE_DESCRIPTION("LAB_04 (Part 2): seqfile");



#define FORTUNE_DIRNAME "fortune_dir"
#define FORTUNE_FILENAME "fortune_file"
#define FORTUNE_SYMLINK "fortune_symlink"
#define FORTUNE_PATH FORTUNE_DIRNAME "/" FORTUNE_FILENAME

#define COOKIE_MAX_BUF_SIZE PAGE_SIZE // 4096 байт


// Базовая структура для работы с файловой системой proc
static struct proc_dir_entry *fortuneDir;
static struct proc_dir_entry *fortuneFile;
static struct proc_dir_entry *fortuneSymlink;

// Хранилище для фортунок
static char *buffer = NULL;

static int readInd = 0;
static int writeInd = 0;


static int showFortune(struct seq_file *seqFile, void *v)
{
    int len = strlen(buffer + readInd);

    printk(KERN_INFO "+ FortuneSEQ (show): %s called\n", __func__);

    if (readInd >= writeInd)
    {
        readInd = 0;
    }

    // seq_printf - действия аналогичны copy_to_user
    // Эквивалент printf для seq_file
    // Принимает: строку и доп аргументы значений, структуру seq_file 
    // Если вернула не ноль, то это значит, что буфер заполнен
    // и вывод будет отброшен
    seq_printf(seqFile, buffer + readInd);


    if (len > 0)
    {
        readInd += len + 1;
    }

    return 0;
}



static int openFortune(struct inode *spInode, struct file *spFile)
{
    printk(KERN_INFO "+ FortuneSEQ (open): %s called\n", __func__);

    // Для создания одного файлового экземпляра модуля
    // При передаче в функцию, showFortune - передает адрес
    // страницы памяти
    return single_open(spFile, showFortune, NULL);
}


static int releaseFortune(struct inode *spInode, struct file *spFile)
{
    printk(KERN_INFO "+ FortuneSEQ (release): %s called\n", __func__);

    return single_release(spInode, spFile);
}


// Запись в ядро
// 1 - указатель на структуру file
// 2 - буфер данныъ длдя чтения
// 3 - размер передаваемых данных
// 4 - смещение от начала данных файла для операции чтения
static ssize_t writeFortune(struct file *file, const char __user *buf, size_t len, loff_t *fPos)
{
    printk(KERN_INFO "+ FortuneSEQ (write): %s called\n", __func__);


    // Проверка достаточности места для фортунки
    if (len > COOKIE_MAX_BUF_SIZE - writeInd + 1)
    {
        printk(KERN_ERR "+ FortuneSEQ (write): Buffer is overflowed\n");
        return -ENOSPC;
    }

    // Копирование буфера из пространства пользователя
    // в пространоство ядра
    // 1 - место в пространстве ядра
    // 2 - место из пространства пользователя
    // 3 - кол-во байт
    if (copy_from_user(&buffer[writeInd], buf, len) != 0)
    {
        printk(KERN_ERR "+ FortuneSEQ (write): copy_from_user returned error\n");
        return -EFAULT;
    }

    writeInd += len;
    buffer[writeInd - 1] = '\n';

    buffer[writeInd] = '\0';
    writeInd += 1;

    // Кол-во символов, записанных в буфер (cookieBuffer)
    return len;
}


static const struct proc_ops fops =
{
    proc_read: seq_read,
    proc_write: writeFortune,
    proc_open: openFortune,
    proc_release: releaseFortune,
};


static void cleanUpFortune(void)
{
    printk(KERN_INFO "+ FortuneSEQ (cleanUp): %s called\n", __func__);

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

    vfree(buffer);
}


static int __init initFortune(void)
{
    printk(KERN_INFO "+ FortuneSEQ (init): %s called\n", __func__);

    // Выделить виртуально непрерывный блок памяти
    if ((buffer = vmalloc(COOKIE_MAX_BUF_SIZE)) == NULL)
    {
        printk(KERN_ERR "+ FortuneSEQ (init): Allocate memory error\n");
        return -ENOMEM;
    }

    memset(buffer, 0, COOKIE_MAX_BUF_SIZE);

    // Указатель на proc_dir_entry (корневой каталог)
    // Имя самой директории
    if ((fortuneDir = proc_mkdir(FORTUNE_DIRNAME, NULL)) == NULL)
    {
        printk(KERN_ERR "+ FortuneSEQ (init): Create dir in proc error\n");
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
        printk(KERN_ERR "+ FortuneSEQ (init): Create file in proc error\n");
        cleanUpFortune();
        return -ENOMEM;
    }

    
    // Создание символической ссылки
    // Имя ссылки
    // Указатель на корневой каталог /proc
    // Имя файла, на который указывает ссылка
    if ((fortuneSymlink = proc_symlink(FORTUNE_SYMLINK, NULL, FORTUNE_PATH)) == NULL)
    {
        printk(KERN_ERR "+ FortuneSEQ (init): Create symlink in proc error\n");
        cleanUpFortune();
        return -ENOMEM;
    }

    // Начальные значения
    readInd = 0;
    writeInd = 0;

    printk(KERN_INFO "+ FortuneSEQ (init): Module loadded with no errors\n");

    return 0;
}


static void __exit exitFortune(void)
{
    printk(KERN_INFO "+ FortuneSEQ (exit): %s called\n", __func__);
    cleanUpFortune();

    printk(KERN_INFO "+ FortuneSEQ (exit): Module removed");
}


module_init(initFortune);
module_exit(exitFortune);
