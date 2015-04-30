#include "customlinkedlist.h"


List* liste_init(){
	List* l = (List*)xmalloc(sizeof(List));
	l->tete=NULL;
	l->queue=NULL;
	l->size=0;
	return l;
}

void liste_view(List* l){
	if(l->size!=0){
		Elem* e1 = l->tete;
		while(e1){
			printf("elem %d : %s\n",e1->number,e1->res );
			e1 = e1->suivant;
		}
	}
}


void liste_dest(List* l){
	if(l->size!=0){
		Elem* e1 = l->tete;
		Elem* e2 = NULL;
		int first=1;
		while(liste_a_un_suivant(l,e1) || first>0){
			e2=e1;
			if(first>0){
				first=0;
			}else{
				e2=e1;
				e1 = liste_suivant(l,e1);
				elem_dest(e2);
			}
		}
	}
	free(l);
}



void liste_add_last(List* l,int number, char* res){
	Elem* el = elem_init(number,res);
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

Elem* liste_tete(List* l){
	return l->tete;
}

Elem* liste_get(List* l, int nb){
	Elem* e=l->tete;
	if(nb>=l->size){
		return NULL;
	}
	while(e && e->number!=nb){
		e=e->suivant;
	}
	return e;
}


int liste_a_un_suivant(List* l, Elem* el){
	return el->suivant!=NULL;
}

Elem* liste_suivant(List* l, Elem* el){
	return el->suivant;	
}


void liste_supprime(List* l, Elem* el){
	Elem* prec = el->precedent;
	Elem* suc = el->suivant;
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


/**************************************/
/*              ELEM                  */
/**************************************/


Elem* elem_init(int number, char* res){
	Elem* elem = (Elem*)xmalloc(sizeof(Elem));
	elem->precedent=NULL;
	elem->suivant=NULL;
	elem->number=number;
	elem->res=res;
	return elem;
}

void elem_dest(Elem* el){
	free(el);
}

