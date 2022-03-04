#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/limits.h>

#include "lib/stack.h"
#include "lib/statistics.h"
#include "errno.h"

#include <time.h>


#define FTW_FILE 1
#define FTW_DIR  2


// Стек -- список
node_t *stk = NULL;

// // Стек -- массив
//struct stack stk;
static stat_t stats;


typedef int fn(const char *, int, int);



int doPath(fn *func, char *fullPath, int depth)
{
    if (depth < 0)
    { 
        chdir(fullPath);
        return 0;
    }

    struct stat statBuf;

    struct dirent *dirp;
    DIR *dp; // тип данных для потока каталога

    // lstat -- возвращает информацию о файле и записывает в буфер 
    // (в отличие от stat -- в случае символьных ссылок возвращает 
    // информацию о самой ссылке, а не о файле, на который указывает)
    if (lstat(fullPath, &statBuf) == -1)
    {
        return -1;
    }

    // Если не каталог
    if (!S_ISDIR(statBuf.st_mode))
    {
        // Статистика файлов
        incStat(&statBuf, &stats);
        func(fullPath, FTW_FILE, depth);

        return 0;
    }

    // Если каталог
    func(fullPath, FTW_DIR, depth);
    // Инкремент каталогов
    stats.ndir++;

    // opendir() -- открывает поток каталога 
    // для чтения каталога
    if ((dp = opendir(fullPath)) == NULL)
    {
        printf("\nОшибка: каталог не доступен для открытия\n");
        return -1;
    }

    // Переход в каталог
    if (chdir(fullPath) == -1)
    {
        // Закрыть поток при неудаче перехода
        closedir(dp);
        printf("\nОшибка: не удалось перейти в каталог\n");

        return -1;
    }

    // Инкремент глубины дерева каталогов
    depth++;


    // Для возврата
    // Подняться на уровень выше при завершении
    // обработки текущей директории
    node_t *elem = create_node("..", -1);

    push(&stk, elem);

    // readdir() -- читает записи в каталоге
    // и возвращает указатель на структуру dirent
    // или NULL, если все записи прочитаны
    while ((dirp = readdir(dp)) != NULL)
    {   
        // Пропуск текущего и родительского каталогов
        if ((strcmp(dirp->d_name, ".") != 0) && (strcmp(dirp->d_name, "..") != 0))
        {
            // Добавить информацию о найденном каталоге
            elem = create_node(dirp->d_name, depth);

            push(&stk, elem);
        }
    }

    // Если readir() вернуло NULL и при этом errno == 0,
    // то в каталоге больше не осталось записей
    if (errno != 0)
    {
        printf("\nОшибка: не удалость прочесть все записи\n");
    }

    // Закрыть поток каталога
    if (closedir(dp) == -1)
    {
        return -1;
    }

    return 0;
}


static int ftw(char *pathName, fn *func)
{
    // Изменить текущую директорию на переданную
    if (chdir(pathName) == -1)
    {
        printf("\nОшибка: каталога не существует или нет доступа\n");
        return -1;
    }

    
    // PATH_MAX = 4096 -- максимальное размер имени пути
    char cwd[PATH_MAX];

    // getcwd() -- копирует абсолютный путь к текущему
    // каталогу в буфер размера size

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        printf("\nError: Can not get full path to work dir\n");
        return -1;
    }
    

    // Добавить запись в стек 
    node_t *elem = create_node(cwd, 0);
    push(&stk, elem); 

    while (!isEmpty(&stk))
    {
        // Проход по файлам каталога
        doPath(func, elem->fileName, elem->depth);
        elem = pop(&stk);
    }

    // Распечатать статистику файлов
    printStat(&stats);

    return 0;
}



static int function(const char *pathName, int type, int depth)
{
    for (int i = 0; i < depth; i++)
    {
        printf(MAGENTA "----| " RESET);
    }

    if (type == FTW_DIR)
    {
        printf(GREEN" %s\n" RESET,  pathName);
    }
    else if (type == FTW_FILE)
    {
        printf(CYAN " %s\n" RESET,  pathName);
    }

    return 0;
}




int main(int argc, char *argv[])
{
    if (argc != 2) 
    {
        printf("Error: directory is not set");
        return -1;
    }

    return ftw(argv[1], function);
}

