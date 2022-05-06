#include <fcntl.h>
#include <unistd.h> // read, write.
#include <pthread.h>
#include <stdio.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"



static int filePos = 0;
pthread_mutex_t mutex;


int outputFile(int fd, int threadNum)
{
	pthread_mutex_lock(&mutex);

    int rc = 1;
    char c;

    lseek(fd, filePos, SEEK_SET);
    rc = read(fd, &c, 1);

    if (rc == 1)
    {
        if (threadNum == 1)
            printf(RED);
        else
            printf(GREEN);

        write(1, &c, 1);

        printf(RESET);
    }

    filePos++;

	pthread_mutex_unlock(&mutex);

    return rc;
}

void *thr_fn(void *arg)
{
	int fd = open("alphabet.txt", O_RDONLY);
    int rc = 1;

    while (rc == 1)
    {
	    rc = outputFile(fd, 2);
        sleep(1);
    }
}

int main()
{
    setbuf(stdout, NULL);

	pthread_t tid;
    int rc = 1;

	int fd = open("alphabet.txt", O_RDONLY);

	int err = pthread_create(&tid, NULL, thr_fn, 0);
	if (err)
	{
		printf("Unable to create a thread");
		return -1;
	}
    
    while (rc == 1)
    {
	    rc = outputFile(fd, 1);
        sleep(1);
    }

	pthread_join(tid, NULL);

    write(1, "\n", 1);

	return 0;
}