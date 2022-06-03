#ifndef _SOCKET_H_
#define _SOCKET_H_

#define SOCK_NAME "socket.soc"

#define MAX_MSG_LEN 128
#define MAX_LEN_ERR_MSG 256

#define LEN_STRUCT_SOCKADDR(a) strlen(a.sa_data) + sizeof(a.sa_family)

#define TRUE 1
#define FALSE 0

#endif