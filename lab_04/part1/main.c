#include "lib/info.h"



void preparePrint(FILE **f_read, char *path, FILE **f_out, char *foutName)
{
    // Открытие файлов
    *f_read = fopen(path, "r");

    if (f_read == NULL)
    {
        printf("\nError: Can not open file %s\n", path);
        exit(-1);
    }

    *f_out = fopen(foutName, "a");

    if (f_out == NULL)
    {
        printf("\nError: Can not open file %s\n", foutName);
        fclose(*f_read);
        exit(-1);
    }

    // Оглавление
    fprintf(*f_out, "\n\n\n--------------------------------- \
                        \n   %s \
                        \n---------------------------------\n\n", path);
} 






void infoFromFile(char *readfile, char *fileout, char *path)
{
    char buf[BUF_SIZE] = "\0";
    int len, i;
    FILE *f_read, *f_out;

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);
    
    // Вывод
    while ((len = fread(buf, 1, BUF_SIZE, f_read)) > 0)
    {
        for (i = 0; i < len; i++)
        {
            if (buf[i] == 0)
                buf[i] = 10;
        }

        fprintf(f_out, "%s", buf);
    }

    // Закрытие файлов
    fclose(f_read);
    fclose(f_out);    
}


// environ, cmdline, io, comm, maps, smaps
void infoProcessFromFile(char *readfile, char *fileout)
{
    char path[PATH_LENGTH] = "\0";
    getPath(path, readfile);

    infoFromFile(readfile, fileout, path);
}


// filesystems, interrupts, ioports, modules
void infoGeneralFromFile(char *readfile, char *fileout)
{
    char path[PATH_LENGTH] = "\0";
    getProcPath(path, readfile);

    infoFromFile(readfile, fileout, path);
}



void infoFromFolder(char *readfile, char *fileout, char *path)
{
    FILE *f_read, *f_out;

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);

    // Вывод
    DIR *dir = opendir(path);
    struct dirent *readDir;

    char buf[BUF_SIZE] = "\0";
    char string[PATH_LENGTH];

    while ((readDir = readdir(dir)) != NULL)
    {
        if ((strcmp(readDir->d_name, ".") != 0) && (strcmp(readDir->d_name, "..") != 0))
        {
            fprintf(f_out, "%-23s\n", readDir->d_name);
        }
    }

    // Закрытие файлов
    fclose(f_read);
    fclose(f_out);
}



// task
void infoProcessFromFolder(char *readfile, char *fileout)
{
    char path[PATH_LENGTH] = "\0";
    getPath(path, readfile);

    infoFromFolder(readfile, fileout, path);
}


// fs
void infoGeneralFromFolder(char *readfile, char *fileout)
{
    char path[PATH_LENGTH] = "\0";
    getProcPath(path, readfile);

    infoFromFolder(readfile, fileout, path);
}


// stat, statm
void infoProcessStat(char *readfile, char *fileout, char **meanings)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, readfile);

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);

    // Вывод
    char buf[BUF_SIZE] = "\0";
    fread(buf, 1, BUF_SIZE, f_read);

    char *elem = strtok(buf, " ");

    for (int i = 0; elem != NULL; i += 2)
    { 
        fprintf(f_out, "%s = %s\n", meanings[i], elem);
        elem = strtok(NULL, " ");
    }

    // Закрытие файлов
    fclose(f_read);
    fclose(f_out);
}


// cwd, exe, root
void infoProcessFromFileLink(char *readfile, char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, readfile);

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);

    // Вывод
    char buf[BUF_SIZE] = "\0";

    readlink(path, buf, BUF_SIZE);

    fprintf(f_out, "%s", buf);

    // Закрытие файлов
    fclose(f_read);
    fclose(f_out);
}


// fd
void infoProcessFromFolderLink(char *readfile, char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH] = "\0";
    getPath(path, readfile);

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);

    // Вывод
    DIR *dir = opendir(path);
    struct dirent *readDir;

    char buf[BUF_SIZE] = "\0";
    char string[PATH_LENGTH];

    while ((readDir = readdir(dir)) != NULL)
    {
        if ((strcmp(readDir->d_name, ".") != 0) && (strcmp(readDir->d_name, "..") != 0))
        {
            sprintf(buf, "%s/%s", path, readDir->d_name);
            readlink(buf, string, PATH_LENGTH);

            fprintf(f_out, "{%4s} -- %s\n", readDir->d_name, string);
        }
    }

    // Закрытие файлов
    fclose(f_read);
    fclose(f_out);
}


// pagemap only
void infoPagemap(char *fileout)
{
    FILE *f_out = fopen(fileout, "a");

    if (f_out == NULL)
    {
        printf("\nError: Can not open file %s\n", fileout);
        exit(-1);
    }

    fprintPagemap(f_out); 
    fclose(f_out);
}


int main(int argc, char *argv[])
{
    FILE *f = fopen(argv[1], "w");

    if (f == NULL)
    {
        printf("\nError: Can not open file %s\n", argv[1]);
        exit(-1);
    }

    fclose(f);

    // Вывод информации
    infoProcessFromFile("environ", argv[1]);
    infoProcessFromFile("cmdline", argv[1]);
    infoProcessFromFolderLink("fd", argv[1]);
    infoProcessStat("stat", argv[1], statMeanings);
    infoProcessStat("statm", argv[1], statmMeanings);
    infoProcessFromFile("io", argv[1]);
    infoProcessFromFileLink("cwd", argv[1]);
    infoProcessFromFileLink("exe", argv[1]);
    infoProcessFromFileLink("root", argv[1]);
    infoProcessFromFile("comm", argv[1]);
    infoProcessFromFolder("task", argv[1]);

    infoGeneralFromFile("filesystems", argv[1]);
    infoGeneralFromFolder("fs", argv[1]);
    infoGeneralFromFile("interrupts", argv[1]);
    infoGeneralFromFile("ioports", argv[1]);
    infoGeneralFromFile("modules", argv[1]);


    infoProcessFromFile("maps", argv[1]);
    infoProcessFromFile("smaps", argv[1]);

    infoPagemap(argv[1]);
    
    return 0;
}
