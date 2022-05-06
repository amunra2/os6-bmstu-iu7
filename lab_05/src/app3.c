#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FILE_OUT "res_app3.txt"


void getInfo(long fPos)
{
	struct stat statbuf;

	stat(FILE_OUT, &statbuf);
	printf("inode: %ld\n", statbuf.st_ino);
	printf("Общий размер в байтах: %ld\n", statbuf.st_size);
	printf("Размер блока ввода-вывода: %ld\n", statbuf.st_blksize);
    printf("Текущая позиция: %ld\n\n", fPos);
}


int main()
{
    long fPos;

    FILE *f1 = fopen(FILE_OUT, "w");
    fPos = ftell(f1);
    getInfo(fPos);

    FILE *f2 = fopen(FILE_OUT, "w");
    fPos = ftell(f2);
    getInfo(fPos);

    for (char c = 'A'; c <= 'Z'; c++)
    {
        if (c % 2)
        {
            fprintf(f1, "%c", c);
        }
        else
        {
            fprintf(f2, "%c", c);
        }
    }

    fPos = ftell(f1);
    fclose(f1);
    getInfo(fPos);

    fPos = ftell(f2);
    fclose(f2);
    getInfo(fPos);

    return 0;
}