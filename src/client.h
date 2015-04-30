#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "utils.h"

#define MAX_LENGTH 256


typedef struct client{
	char* prog;
	char* arg1;
	char* arg2;

} Client;

Client* newClient();
void deleteClient(Client* cli);

char* execCli(Client* cli, int nb);

#endif