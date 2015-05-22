#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>

#include "customlinkedlist.h"

typedef struct Generator{
	int number;
	List* results;
	List* runningList;
	FILE* output;
} Generator;


Generator* newGenerator();
void deleteGenerator(Generator* gen);

int getNumber(Generator* gen);
int setResult(Generator* gen, int number, char* result);

/* Si generation de mot au lieu de chiffre, pour plus tard*/
/*char* getString(Generator* gen);*/



void showGenerator(Generator* gen);

#endif