#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include "stack.h"
#include "statistics.h"
#include "errno.h"


#define FTW_FILE 1
#define FTW_DIR  2


static struct stack stk;
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
	DIR *dp;

	if (lstat(fullPath, &statBuf) == -1)
	{
		return -1;
	}

	if (!S_ISDIR(statBuf.st_mode))
	{
		incStat(&statBuf, &stats);
		func(fullPath, FTW_FILE, depth);

		return 0;
	}

	func(fullPath, FTW_DIR, depth);
	stats.ndir++;

	if ((dp = opendir(fullPath)) == NULL)
	{
		printf("\nОшибка: каталог не доступен для открытия\n");
		return -1;
	}

	if (chdir(fullPath) == -1)
	{
		closedir(dp);
		printf("\nОшибка: не удалось перейти в каталог\n");

		return -1;
	}

	depth++;

	// Для возврата
	struct stackElement elem = {.fileName = "..", .depth = -1};

	pushStack(&stk, &elem);

	while ((dirp = readdir(dp)) != NULL)
	{
		if ((strcmp(dirp->d_name, ".") != 0) && (strcmp(dirp->d_name, "..") != 0))
		{
			strcpy(elem.fileName, dirp->d_name);
			elem.depth = depth;

			pushStack(&stk, &elem);
		}
	}


	if (errno == 0)
	{
		printf("В каталоге больше нет записей\n");
	}

	if (closedir(dp) == -1)
	{
		return -1;
	}

	return 0;
}




static int ftw(char *pathName, fn *func)
{
	if (chdir(pathName) == -1)
	{
		printf("\nError: Directory is not exist\n");
		return -1;
	}

	initStack(&stk);

	struct stackElement elem = {.fileName = NULL, .depth = 0};
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		printf("\nError: Can not get full path to work dir\n");
		return -1;
	}

	strcpy(elem.fileName, cwd);
	
	pushStack(&stk, &elem);

	while (!isEmpty(&stk))
	{
		doPath(func, elem.fileName, elem.depth);
		elem = popStack(&stk);
	}

	printStat(&stats);

	return 0;
}




static int function(const char *pathName, int type, int depth)
{
	for (int i = 0; i < depth; i++)
	{
		printf("       |");
	}

	printf("     |--- %s\n",  pathName);

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

