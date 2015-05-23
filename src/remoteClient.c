#include "remoteClient.h"

RemoteClient* newRemoteClient(struct sockaddr_in cli_addr, int dialog_socket){
	static int current_id = 1;
	RemoteClient* rc = NULL;

	char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
	size_t size = sizeof cli_addr;
	socklen_t len = (socklen_t)size;

	int err;
	rc = (RemoteClient*)xmalloc(sizeof(RemoteClient));
	rc->dialog_socket = dialog_socket;
	rc->id = current_id++;
	rc->timestamp_last_pong_sent = (unsigned)time(NULL);
	err = getnameinfo((struct sockaddr*) &cli_addr, len, hbuf, sizeof(hbuf), sbuf,
	                sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
	if (err == 0) {
		rc->cli_addr = cli_addr;
		strcpy(rc->ip, hbuf);
		strcpy(rc->port, sbuf);
		strcpy(rc->data_to_send, "NULL");
	} else {
		//printf("Erreur getnameinfo : %s\n", gai_strerror(err));
	}
	return rc;
}

void deleteRemoteClient(RemoteClient* rc){
	free(rc);
}

void printRemoteClient(RemoteClient* rc){
	printf("RemoteClient %d, socket %d\n",rc->id, rc->dialog_socket);
	//printf("RemoteClient : id = %d, sock : %d\n ip = %s \n port = %s \n data to send = %s \n last pong sent = %d\n\n", rc->id, rc->dialog_socket, rc->ip, rc->port, rc->data_to_send, rc->timestamp_last_pong_sent);
}
