#ifndef REMOTE_CLIENT_H
#define REMOTE_CLIENT_H

#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

typedef struct remoteClient{
	int id;
}RemoteClient;

RemoteClient* newRemoteClient(int id);
void deleteRemoteClient(RemoteClient* rm);
void printRemoteClient(RemoteClient* rm);

#endif