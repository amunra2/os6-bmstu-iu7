#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>

#define FILE_OUT "res_app3_threads.txt"


void getInfo(long fPos)
{
	struct stat statbuf;

	stat(FILE_OUT, &statbuf);
	printf("inode: %ld\n", statbuf.st_ino);
	printf("Общий размер в байтах: %ld\n", statbuf.st_size);
	printf("Размер блока ввода-вывода: %ld\n", statbuf.st_blksize);
	printf("Текущая позиция: %ld\n\n", fPos);
}

void writeChar(char c)
{
	long fPos;
	FILE *f = fopen(FILE_OUT, "a");
	fPos = ftell(f);
	getInfo(fPos);

	while (c <= 'Z')
	{
		fprintf(f, "%c", c);
		c += 2;
	}

	fPos = ftell(f);
	fclose(f);
	getInfo(fPos);
}

void *launchThread(void *arg)
{
	writeChar('A');
}

int main()
{
    pthread_t thread;
    int rc = pthread_create(&thread, NULL, launchThread, NULL);

    if (rc)
	{
		printf("Unable to create a thread");
		return -1;
	}

    writeChar('B');

    pthread_join(thread, NULL);
    return 0;
}
