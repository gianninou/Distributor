#include "remoteClient.h"

RemoteClient* newRemoteClient(struct sockaddr_in dialog_socket){
	static int current_id = 0;
	RemoteClient* rc = NULL;

	char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
	size_t size = sizeof dialog_socket;
	socklen_t len = (socklen_t)size;

	if (getnameinfo((struct sockaddr*) &dialog_socket, len, hbuf, sizeof(hbuf), sbuf,
	                sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0) {
		rc = (RemoteClient*)xmalloc(sizeof(RemoteClient));
		rc->id = current_id++;
		rc->dialog_socket = dialog_socket;
		strcpy(rc->ip, hbuf);
		strcpy(rc->port, sbuf);
		strcpy(rc->data_to_send, "NULL");
		rc->timestamp_last_pong_sent = -1;
	}
	return rc;
}

void deleteRemoteClient(RemoteClient* rc){
	free(rc);
}

void printRemoteClient(RemoteClient* rc){
	printf("RemoteM : id = %d\n ip = %s \n port = %s \n data to send = %s \n last pong sent = %d\n", rc->id, rc->ip, rc->port, rc->data_to_send, rc->timestamp_last_pong_sent);
}
