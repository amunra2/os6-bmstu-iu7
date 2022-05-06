#include <linux/fs.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define OK 0
#define FILE_NAME "alphabet.txt"
#define BUFF_SIZE 18

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"


int main()
{
	int fd = open(FILE_NAME, O_RDONLY); // O_RDONLY - только на чтение.

	FILE *fs1 = fdopen(fd, "r");
	char buf1[BUFF_SIZE];

	// Функция изменяет буфер, который будет использоваться для 
	// операций ввода/вывода с указанным потоком. 
	// Эта функция позволяет задать режим доступа и размер буфера. 
	setvbuf(fs1, buf1, _IOFBF, BUFF_SIZE); // _IOFBF - блочная буферизация.

	FILE *fs2 = fdopen(fd, "r");
	char buf2[BUFF_SIZE];
	setvbuf(fs2, buf2, _IOFBF, BUFF_SIZE);

	int rc1 = 1, rc2 = 1;

    printf("\nResult:\n");

	while (rc1 == 1 || rc2 == 1)
	{
		char c;
		rc1 = fscanf(fs1, "%c", &c);

		if (rc1 == 1)
		{
			fprintf(stdout, RED "%c" RESET, c);
		}

		rc2 = fscanf(fs2, "%c", &c);

		if (rc2 == 1)
		{
			fprintf(stdout, GREEN "%c" RESET, c);
		}
	}

	printf("\n\n");
	return OK;
}