#ifndef REMOTE_CLIENT_H
#define REMOTE_CLIENT_H

#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#include "utils.h"

#define BUFFERSIZE 256

typedef struct remoteClient{
	int id;
	struct sockaddr_in dialog_socket;
	char ip[NI_MAXHOST];
	char port[NI_MAXSERV];
	char data_to_send[BUFFERSIZE];
	int timestamp_last_pong_sent;

}RemoteClient;

RemoteClient* newRemoteClient(struct sockaddr_in dialog_socket);
void deleteRemoteClient(RemoteClient* rm);
void printRemoteClient(RemoteClient* rm);

#endif
