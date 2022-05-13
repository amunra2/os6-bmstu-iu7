#include <fcntl.h>
#include <unistd.h> // read, write.
#include <pthread.h>
#include <stdio.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"

pthread_mutex_t mutex;


void *thr_fn(void *arg)
{
	int fd = open("alphabet.txt", O_RDONLY);
    int rc = 1;
    char c;

    pthread_mutex_lock(&mutex);
    while ((rc = read(fd, &c, 1)) == 1)
    {
	    write(1, &c, 1);
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{
	pthread_t tid;
    int rc = 1;
    char c;

	int fd = open("alphabet.txt", O_RDONLY);

	int err = pthread_create(&tid, NULL, thr_fn, 0);
	if (err)
	{
		printf("Unable to create a thread");
		return -1;
	}

    write(1, "\n", 1);
    
    pthread_mutex_lock(&mutex);
    while ((rc = read(fd, &c, 1)) == 1)
    {
	    write(1, &c, 1);
    }
    pthread_mutex_unlock(&mutex);

	pthread_join(tid, NULL);

    write(1, "\n", 1);

	return 0;
}