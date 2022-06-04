#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "socket.h"


void createMessage(char buf[MAX_MSG_LEN], int argc, char *argv[])
{
	long int curr_time = time(NULL); // Считываем текущее время.

	sprintf(buf, "\nPID: %d\nTime: %s Message: ", getpid(), ctime(&curr_time));

	if (argc < 2)
		strcat(buf, "Tsvetkov IU7-63B\n");
	else // если передано сообщение через аргумент командной строки
		strcat(buf, argv[1]);
}


int main(int argc, char *argv[])
{
	struct sockaddr srvr_name; // Данные об адресе сервера
	int sock;				   // Дескриптор сокета

	char buf[MAX_MSG_LEN];
	createMessage(buf, argc, argv);

	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror("Error: socket failed");
		return -1;
	}

	srvr_name.sa_family = AF_UNIX;
	strcpy(srvr_name.sa_data, SOCK_NAME);

	if (connect(sock, (struct sockaddr *)&srvr_name, sizeof(srvr_name)) < 0)
	{
		printf("Error: connect() failed");
		return -1;
	}
	else
	{
		printf("Client connected!\n");
	}

	// 0 - доп флаги
	if (send(sock, buf, strlen(buf) + 1, 0) < 0)
	{
		perror("Error: sendto fail"); 
		return -1;
	}

	// Server's response
	int namelen;
	int bytes;
	char bufServer[BUFSIZ];

	bytes = recv(sock, bufServer, sizeof(bufServer), 0);

	printf("\nReceived Server message: %s", bufServer);

	close(sock);
	printf("Success send!\n");

	return 0;
}