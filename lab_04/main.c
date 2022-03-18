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


void infoEnviron(char *fileout)
{
    char buf[BUF_SIZE] = "\0";
    int len, i;
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, "environ");

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



void infoCmdline(char *fileout)
{
    char buf[BUF_SIZE] = "\0";
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, "cmdline");

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);
    
    // Вывод
    fread(buf, 1, BUF_SIZE, f_read);
    fprintf(f_out, "%s", buf);

    // Закрытие файлов
    fclose(f_read);
    fclose(f_out);
}


void infoFD(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, "fd");

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
            sprintf(buf, "%s%s", path, readDir->d_name);
            readlink(buf, string, PATH_LENGTH);
            fprintf(f_out, "{%4s} -- %s\n", readDir->d_name, string);
        }
    }

    // Закрытие файлов
    fclose(f_read);
    fclose(f_out);
}




void infoStat(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, "stat");

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);

    // Вывод
    char buf[BUF_SIZE] = "\0";
    fread(buf, 1, BUF_SIZE, f_read);

    char *elem = strtok(buf, " ");

    for (int i = 0; elem != NULL; i += 2)
    {
        fprintf(f_out, "%s = %s\n", statMeanings[i], elem);
        elem = strtok(NULL, " ");
    }


    // Закрытие файлов
    fclose(f_read);
    fclose(f_out);
}


void infoStatm(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, "statm");

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);
    
    // Вывод
    char buf[BUF_SIZE] = "\0";
    fread(buf, 1, BUF_SIZE, f_read);

    char *elem = strtok(buf, " ");

    for (int i = 0; elem != NULL; i += 2)
    {
        fprintf(f_out, "%s = %s\n", statmMeanings[i], elem);
        elem = strtok(NULL, " ");
    }

    // Закрытие файлов
    fclose(f_read);
    fclose(f_out);
}



void infoIO(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, "io");

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);

    // Вывод
    int len, i;
    char buf[BUF_SIZE] = "\0";


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



void infoCWD(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, "cwd");

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


void infoEXE(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, "exe");


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


void infoRoot(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, "root");

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


void infoComm(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, "comm");

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);

    // Вывод
    char buf[BUF_SIZE] = "\0";

    fread(buf, 1, BUF_SIZE, f_read);
    fprintf(f_out, "%s", buf);

    // Закрытие файлов
    fclose(f_read);
    fclose(f_out);
}


void infoTask(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, "task");

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
            sprintf(buf, "%s%s", path, readDir->d_name);
            readlink(buf, string, PATH_LENGTH);
            fprintf(f_out, "{%4s} -- %s\n", readDir->d_name, string);
        }
    }

    // Закрытие файлов
    fclose(f_read);
    fclose(f_out);
}


void infoMaps(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, "maps");

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);

    // Вывод
    int len, i;
    char buf[BUF_SIZE] = "\0";

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




void infoSmaps(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getPath(path, "smaps");

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);

    // Вывод
    int len, i;
    char buf[BUF_SIZE];

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



// Общая информация

void infoFilesystems(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getProcPath(path, "filesystems");

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);

    // Вывод
    int len, i;
    char buf[BUF_SIZE];

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



void infoFS(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getProcPath(path, "fs");

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
            sprintf(buf, "%s%s", path, readDir->d_name);
            readlink(buf, string, PATH_LENGTH);
            fprintf(f_out, "{%4s} -- %s\n", readDir->d_name, string);
        }
    }

    // Закрытие файлов
    fclose(f_read);
    fclose(f_out);
}



void infoInterrupts(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getProcPath(path, "interrupts");

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);

    // Вывод
    int len, i;
    char buf[BUF_SIZE];

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


void infoIOports(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getProcPath(path, "ioports");

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);

    // Вывод
    int len, i;
    char buf[BUF_SIZE];

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


void infoModlules(char *fileout)
{
    FILE *f_read, *f_out;
    char path[PATH_LENGTH];
    getProcPath(path, "modules");

    // Открыть файлы и напечатать стартовую строку
    preparePrint(&f_read, path, &f_out, fileout);

    // Вывод
    int len, i;
    char buf[BUF_SIZE];

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
    infoEnviron(argv[1]);
    infoCmdline(argv[1]);
    infoFD(argv[1]);
    infoStat(argv[1]);
    infoStatm(argv[1]);
    infoIO(argv[1]);
    infoCWD(argv[1]);
    infoEXE(argv[1]);
    infoRoot(argv[1]);
    infoComm(argv[1]);
    infoTask(argv[1]);
    infoFilesystems(argv[1]);
    infoFS(argv[1]);
    infoInterrupts(argv[1]);
    infoIOports(argv[1]);
    infoModlules(argv[1]);

    infoMaps(argv[1]);
    infoSmaps(argv[1]);
    infoPagemap(argv[1]);
    
    
    return 0;
}
