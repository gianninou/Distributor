#ifndef CLIENT_EXEC_H
#define CLIENT_EXEC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "utils.h"

#define MAX_LENGTH 256


typedef struct clientExec{
	char* prog;
	char* arg1;
	char* arg2;
} ClientExec;

ClientExec* newClientExec();
void deleteClientExec(ClientExec* cli);

char* execClientExec(ClientExec* cli, int nb);

#endif