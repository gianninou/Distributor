#include "client.h"


Client* newClient(){
	Client* cli = (Client*)xmalloc(sizeof(Client));
	cli->prog = "python";
	cli->arg1 = "./algo/friedman.py";
	cli->arg2 = (char*)xmalloc(sizeof(char)*20);
	return cli;
}


void deleteClient(Client* cli){
	free(cli->prog);
	free(cli->arg1);
	free(cli->arg2);
	free(cli);
}


char* execCli(Client* cli, int nb){
	char* result = (char*)malloc(sizeof(char)*MAX_LENGTH);
	memset(result,0,MAX_LENGTH);

	int fd[2];
	pipe(fd);
	if(!fork()){
		/* fils */
		close(fd[0]);
		dup2(fd[1],1);
		sprintf(cli->arg2,"%d",nb);
		execlp(cli->prog,cli->prog,cli->arg1,cli->arg2,NULL);
		perror("Erreur d'execution");
		exit(0);
	}
	close(fd[1]);

	/* TODO check return */
	int pid;
	wait(&pid);

	int r = read(fd[0],result,MAX_LENGTH-1);
	/*printf("RESULTAT %d %d : '%s'\n", nb, r, result);*/
	if(r){
		return result;
	}else{
		free(result);
		return NULL;
	}
}



int main(int argc, char* argv[]){
	printf("Lancement du client ...\n");

	Client* cli = newClient();

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
		char* res = execCli(cli,i);
		/* On récupere le resultat du cli ext, on l'envoie au serveur (et on s'assure de la bonne reception en option) */
		if(res){
			printf("RES  '%s'\n", res);
		}else{
			printf("NULL\n");
		}


	}


	return 0;
}