#include "remoteClient.h"

RemoteClient* newRemoteClient(int id){
	RemoteClient* rm = (RemoteClient*)xmalloc(sizeof(RemoteClient));
	rm->id=id;
	return rm;
}


void deleteRemoteClient(RemoteClient* rm){
	free(rm);
}


void printRemoteClient(RemoteClient* rm){
	printf("RM : id=%d\n",rm->id );
}

