#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "generator.h"
#include "utils.h"
#include "listRemoteClient.h"
#include "remoteClient.h"

#define MAX_CLIENTS 100
#define BUFF_LEN 100

int adpu(Generator* gen, char* message, char* reponse);

#endif
