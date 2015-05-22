#ifndef LIST_REMOTE_CLIENT_H
#define LIST_REMOTE_CLIENT_H


#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "remoteClient.h"

typedef struct elementRemoteClient{
	struct elementRemoteClient* precedent;
	struct elementRemoteClient* suivant;

	RemoteClient* remoteClient;
} ElemRemoteClient;

typedef struct listRemoteClient{
	ElemRemoteClient* tete;
	ElemRemoteClient* queue;
	int size;
} ListRemoteClient;


ListRemoteClient* listeRemote_init();
void listeRemote_dest(ListRemoteClient* l);


void listeRemote_add_last(ListRemoteClient* l,RemoteClient* rc);

ElemRemoteClient* listeRemote_tete(ListRemoteClient* l);
ElemRemoteClient* listeRemote_get(ListRemoteClient* l, int id);
int listeRemote_a_un_suivant(ListRemoteClient* l, ElemRemoteClient* el);
ElemRemoteClient* listeRemote_suivant(ListRemoteClient* l, ElemRemoteClient* el);
void listeRemote_supprime(ListRemoteClient* l, ElemRemoteClient* el);
void listeRemote_print(ListRemoteClient* l);
int listeRemote_client_exists(ListRemoteClient* l, RemoteClient* rc);
int listeRemote_get_size(ListRemoteClient* l);
int listeRemote_get_i_socket(ListRemoteClient* l, int i);

/*void liste_free(ListRemoteClient* l);*/

ElemRemoteClient* elemRemote_init(RemoteClient* rm);
void elemRemote_dest(ElemRemoteClient* el);

#endif
