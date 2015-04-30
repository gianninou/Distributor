#include "generator.h"


Generator* newGenerator(){
	Generator* gen = (Generator*)malloc(sizeof(Generator));
	gen->number=0;
	gen->results = liste_init();
	gen->runningList = liste_init();
	gen->output = fopen("output.txt","w+");
	if(!gen->output){
		perror("Erreur ouverture output.txt");
		exit(1);
	}
	return gen;
}


void deleteGenerator(Generator* gen){
	/*liste_free(gen->results);*/
	fclose(gen->output);
	free(gen);
}

int getNumber(Generator* gen){
	/*printf("Actuel : %d\n",gen->number);*/
	gen->number++;
	liste_add_last(gen->runningList,gen->number,NULL);
	return gen->number;
}

void setResult(Generator* gen, int number, char* result){
	/*TODO ameliorer le malloc*/
	Elem* elem = liste_get(gen->runningList,number);
	if (elem){
		printf("Acceptation de %d : %s\n",number, result );
		liste_supprime(gen->runningList,elem);
		char* str = (char*)xmalloc(sizeof(char)*(strlen(result)+100));
		sprintf(str,"%d : %s\n",number,result);
		fputs(str,gen->output);
		free(str);
		liste_add_last(gen->results,number,result);	
	}else{
		printf("Pas de calcul en attente sur %d\n", number);
	}
}

void showGenerator(Generator* gen){
	printf("------------------------------\n");
	printf("Liste : \n");
	if(gen->results->size){
		liste_view(gen->results);	
	}else{
		printf("Vide\n");
	}
	
	printf("Running List\n");
	if(gen->runningList->size){
		liste_view(gen->runningList);	
	}else{
		printf("Vide\n");
	}
	printf("------------------------------\n");
}
