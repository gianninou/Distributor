#include "test.h"

void testCliExt(){


}

void testRemoteClient(){
	ListRemoteClient* l = listeRemote_init();
	RemoteClient* rc1 = (RemoteClient*)malloc(sizeof(RemoteClient));
	rc1->id=1;
	RemoteClient* rc2 = (RemoteClient*)malloc(sizeof(RemoteClient));
	rc2->id=2;
	RemoteClient* rc3 = (RemoteClient*)malloc(sizeof(RemoteClient));
	rc3->id=3;
	listeRemote_view(l);
	listeRemote_add_last(l,rc1);
	listeRemote_view(l);
	listeRemote_add_last(l,rc2);
	listeRemote_view(l);
	listeRemote_add_last(l,rc3);
	listeRemote_view(l);
	listeRemote_supprime(l,listeRemote_get(l,rc2->id));
	listeRemote_view(l);
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
	setResult(gen, 18, "tutu");
	showGenerator(gen);
	printf("Génération de %d\n", getNumber(gen));
	
	showGenerator(gen);
}

int main(int argc, char* argv[]){
	testRemoteClient();
	/*testGen();
	testCliExt();*/
	return 0;
}


