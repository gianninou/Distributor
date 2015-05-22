#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>


#include "generator.h"
#include "utils.h"
#include "listRemoteClient.h"
#include "remoteClient.h"
#include "customlinkedlist.h"

#define PORT_MULTICAST 10000
#define GROUP_MULTICAST "239.13.37.92"

#define MAX_CLIENTS 100
#define BUFF_LEN 100

int apdu(Generator* gen, List* liste, char* message, char* reponse);
void *thread_ping(void *arg);

#endif
