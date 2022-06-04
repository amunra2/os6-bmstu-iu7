#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#include "socket.h"


static int sock; // Дескриптор сокета.


void cleanup(int sock)
{
    close(sock);
    unlink(SOCK_NAME);
}


void sighandler(int signum) { 
    cleanup(sock);
    exit(0);
}


int main(int argc, char *argv[])
{
    struct sockaddr srvr_name; // Данные об адресе сервера
    struct sockaddr rcvr_name; // Данные об адресе клиента, запросившего соединение
    int namelen;			   // Длина возвращаемой структуры с адресом

    char buf[MAX_MSG_LEN]; // Буфер, в который записываются сообщения от клиентов
    int bytes;			   // Кол-во полученных байт

    long int curr_time = time(NULL); // Считываем текущее время.

    // Создаем сокет. Получаем дескриптор сокета
    // AF_UNIX - сокеты в файловом пространстве имен
    // SOCK_DGRAM - датаграмный сокет. Хранит границы сообщений
    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("Error: socket failed");
        return -1;
    }

    srvr_name.sa_family = AF_UNIX;
    strcpy(srvr_name.sa_data, SOCK_NAME); // sa_data - имя файла сокета.

    // bind() - связывает сокет с заданным адресом.
    // После вызова bind() программа-сервер становится доступна для соединения
    // по заданному адресу (имени файла)
    if (bind(sock, &srvr_name, LEN_STRUCT_SOCKADDR(srvr_name)) < 0)
    {
        perror("bind failed");
        return -1;
    }

    if (signal(SIGINT, sighandler) == SIG_ERR) {
        perror("Error: signal call error");
        cleanup(sock);
        return -1;
    }



    if (listen(sock, 1) < 0)
	{
		printf("Error: listen() failed\n");
		return -1;
	}


    char bufSend[BUFSIZ];
    sprintf(bufSend, "\nServer Answer - PID: %d\n", getpid());

    while (TRUE)
    {
        int newsock = accept(sock, NULL, NULL);

        if (newsock < 0)
        {
            printf("Error: accept() failed\n");
            return -1;
        }


        // recvfrom блокирует программу до тех пор, пока на входе не появятся новые данные.
        // bytes = recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL); // В нашем случае можно и вот так.
        bytes = recv(newsock, buf, sizeof(buf), 0);

        // Response to the client
        if (send(newsock, bufSend, strlen(bufSend) + 1, 0) < 0)
        {
            perror("Error: sendto fail"); 
            return -1;
        }

        printf("\n\nReceived message: %s", buf);
    }

    return 0;
}
