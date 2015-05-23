#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>

#include <getopt.h>
#include <pthread.h>

#include "clientExec.h"

#define PORT_MULTICAST 10000
#define GROUP_MULTICAST "239.13.37.92"

#define DELIMITER "|"

typedef struct{
	int socket;
}DATA;

void lock();
void unlock();

#endif