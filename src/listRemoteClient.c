#include "listRemoteClient.h"


ListRemoteClient* listeRemote_init(){
	ListRemoteClient* l = (ListRemoteClient*)malloc(sizeof(ListRemoteClient));
	l->tete=NULL;
	l->queue=NULL;
	l->size=0;
	return l;
}

void listeRemote_print(ListRemoteClient* l){
	printf("Liste RemoteClient : \n");
	if(l->size!=0){
		ElemRemoteClient* e1 = l->tete;
		while(e1){
			printRemoteClient(e1->remoteClient);
			e1 = e1->suivant;
		}
	}else{
		printf("Vide\n");
	}
}


void listeRemote_dest(ListRemoteClient* l){
	if(l->size!=0){
		ElemRemoteClient* e1 = l->tete;
		ElemRemoteClient* e2=e1;
		while(e2){
			e2=e1->suivant;
			/* free e1->remoteCLient */
			free(e1);
		}
	}
	free(l);
}



void listeRemote_add_last(ListRemoteClient* l,RemoteClient* rm){
	ElemRemoteClient* el = elemRemote_init(rm);
	if(l->size==0){
		l->tete=el;
		l->queue=el;
	}else{
		l->queue->suivant=el;
		el->precedent=l->queue;
		l->queue=el;
	}
	l->size++;
}

ElemRemoteClient* listeRemote_tete(ListRemoteClient* l){
	return l->tete;
}

ElemRemoteClient* listeRemote_get(ListRemoteClient* l, int id){
	ElemRemoteClient* e=l->tete;
	while(e && e->remoteClient->id!=id){
		e=e->suivant;
	}
	return e;
}


int listeRemote_a_un_suivant(ListRemoteClient* l, ElemRemoteClient* el){
	return el->suivant!=NULL;
}

ElemRemoteClient* listeRemote_suivant(ListRemoteClient* l, ElemRemoteClient* el){
	return el->suivant;
}


void listeRemote_supprime(ListRemoteClient* l, ElemRemoteClient* el){
	ElemRemoteClient* prec = el->precedent;
	ElemRemoteClient* suc = el->suivant;
	if(prec == NULL && suc == NULL){
		l->tete=NULL;
		l->queue=NULL;
	}else if(prec == NULL){
		l->tete=suc;
		suc->precedent=NULL;
	}else if(suc==NULL){
		l->queue=prec;
		prec->suivant=NULL;
	}else{
		prec->suivant=suc;
		suc->precedent=prec;
	}
	l->size--;
}

int listeRemote_client_exists(ListRemoteClient* l, RemoteClient* rc) {
	if(l->size == 0) {
		return 0;
	}
	int found = 0;
	ElemRemoteClient* current = l->tete;
	if(rc->id == current->remoteClient->id) {
		return 1;
	}
	int i = 1;
	while(i < l->size && !found) {
		current = current->suivant;
		if(rc->id == current->remoteClient->id) {
			found = 1;
		}
	}
	return found;
}

int listeRemote_get_size(ListRemoteClient* l) {
	return l->size;
}


/**************************************/
/*              ELEM                  */
/**************************************/


ElemRemoteClient* elemRemote_init(RemoteClient* rm){
	ElemRemoteClient* elem = (ElemRemoteClient*)malloc(sizeof(ElemRemoteClient));
	elem->precedent=NULL;
	elem->suivant=NULL;
	elem->remoteClient=rm;
	return elem;
}

void elemRemote_dest(ElemRemoteClient* el){
	free(el);
}
