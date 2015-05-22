#include "generator.h"


Generator* newGenerator(){
	Generator* gen = (Generator*)malloc(sizeof(Generator));
	gen->number=0;
	gen->results = liste_init();
	gen->runningList = liste_init();
	gen->output = fopen("output/output.txt","w+");
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

int setResult(Generator* gen, int number, char* result){
	/*TODO ameliorer le malloc*/
	int res=0;
	Elem* elem = liste_get(gen->runningList,number);
	if (elem){
		if(result[0]!='#'){
			printf("Acceptation de |%d : %s|\n",number, result );
			liste_supprime(gen->runningList,elem);
			char* str = (char*)xmalloc(sizeof(char)*(strlen(result)+100));
			sprintf(str,"%d : %s\n",number,result);
			printf("GENERATOR : |%s|\n",str );
			fputs(str,gen->output);
			fflush(gen->output);
			free(str);
			liste_add_last(gen->results,number,result);	
		}else{
			printf("pas de solution pour %d\n",number );
		}
		res=1;
	}else{
		printf("Pas de calcul en attente sur %d\n", number);
	}
	return res;
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

