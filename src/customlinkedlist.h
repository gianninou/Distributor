#ifndef CUSTOMLINKEDLIST_H
#define CUSTOMLINKEDLIST_H


#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

typedef struct element{
	struct element* precedent;
	struct element* suivant;

	int number;
	char* res;

	
} Elem;

typedef struct custom{
	Elem* tete;
	Elem* queue;
	int size;
} List;


List* liste_init();
void liste_dest(List* l);


void liste_add_last(List* l,int number, char* res);

Elem* liste_tete(List* l);
Elem* liste_get(List* l, int nb);
int liste_a_un_suivant(List* l, Elem* el);
Elem* liste_suivant(List* l, Elem* el);
void liste_supprime(List* l, Elem* el);
void liste_view(List* l);

/*void liste_free(List* l);*/

Elem* elem_init(int number, char* res);
void elem_dest(Elem* el);

#endif
