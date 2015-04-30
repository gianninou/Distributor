#include "main.h"

void testCliExt(){


}


void testGen(){
	Generator* gen = newGenerator();
	printf("Génération de %d\n", getNumber(gen));
	showGenerator(gen);
	printf("Génération de %d\n", getNumber(gen));
	showGenerator(gen);
	printf("Génération de %d\n", getNumber(gen));
	showGenerator(gen);
	printf("Génération de %d\n", getNumber(gen));
	showGenerator(gen);
	setResult(gen, 1, "toto");
	showGenerator(gen);
	printf("Génération de %d\n", getNumber(gen));
	showGenerator(gen);
	printf("Génération de %d\n", getNumber(gen));
	showGenerator(gen);
	printf("Génération de %d\n", getNumber(gen));
	showGenerator(gen);
	printf("Génération de %d\n", getNumber(gen));
	showGenerator(gen);
	setResult(gen, 4, "titi");
	showGenerator(gen);
	setResult(gen, 5, "tata");
	showGenerator(gen);
	setResult(gen, 8, "tutu");
	showGenerator(gen);
	printf("Génération de %d\n", getNumber(gen));
	
	showGenerator(gen);
}

int main(int argc, char* argv[]){
	testGen();
	testCliExt();
	return 0;
}


