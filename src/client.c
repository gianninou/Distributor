#include "client.h"

int main(int argc, char* argv[]){
	printf("Lancement du client ...\n");

	ClientExec* cli = newClientExec();

	int continu=1;
	char buff[MAX_LENGTH];

	/* Initialiser sockets, ... */


	/* s'enregistrer sur le serveur, récuperer son ID */

	/* debut de boucle sur le programme externe */

	while(continu){
		/* on envoie un GET */
		printf("GET\n");

		/* On recupere le nombre du serveur */
		/* TODO mettre un socket */
		fgets(buff,MAX_LENGTH-1,stdin);

		/* .... CALCUL du cli ext */
		int i = atoi(buff);
		char* res = execClientExec(cli,i);
		/* On récupere le resultat du cli ext, on l'envoie au serveur (et on s'assure de la bonne reception en option) */
		if(res){
			printf("RES  '%s'\n", res);
		}else{
			printf("NULL\n");
		}


	}


	return 0;
}