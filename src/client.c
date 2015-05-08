#include "client.h"

int main(int argc, char* argv[]){
	printf("Lancement du client ...\n");

	ClientExec* cli = newClientExec();
	int error;
	int continu=1;
	int r,w;
	char buff[MAX_LENGTH];

	/* Initialiser sockets, ... */


	int serverSocket;
	struct sockaddr_in  serv_addr;

	memset ( (char *) &serv_addr, 0, sizeof(serv_addr) );
	/* Compatibilite IPv6 */
	serv_addr.sin_family = PF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));

	





	serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (serverSocket <0) {
		perror ("erreur socket");
		exit (1);
	}
	/********************************************************************/


	/********************************************************************/

	error = connect (serverSocket, (struct sockaddr *) &serv_addr,  sizeof(serv_addr) );
	if(error < 0){
		perror ("erreur connect");
		exit (1);
	}

	/* s'enregistrer sur le serveur, récuperer son ID */

	/*char buffer[1024];
	memset(buffer,0,1024);
	int count = read(serverSocket,buffer,1024);
	printf("%d : %s\n",count,buffer );
	exit(1);*/

	/* debut de boucle sur le programme externe */
	
	printf("CNX\n");
	w=write(serverSocket,"CNX",3);
	if(w==-1){
		perror("Erreur ecriture");
		exit(1);
	}
	/*fgets(buff,MAX_LENGTH-1,stdin);*/
	r=read(serverSocket,buff,MAX_LENGTH-1);
	if(r==-1){
		perror("Erreur read");
		exit(1);
	}
	printf("recu : %s\n",buff );
	if(strncmp(buff,"COK",3)){
		printf("|%s|\n",buff );
		printf("Erreur de connexion\n");
		exit(1);
	}

	
	while(continu){
		/* on envoie un GET */
		/*printf("GET\n");*/
		w=write(serverSocket,"GET\n",4);
		if(w==-1){
			perror("Erreur ecriture");
			exit(1);
		}

		/* On recupere le nombre du serveur */
		/* TODO mettre un socket */
		/*fgets(buff,MAX_LENGTH-1,stdin);*/
		r=read(serverSocket,buff,MAX_LENGTH-1);
		if(r==-1){
			perror("Erreur read");
			exit(1);
		}
		buff[r]='\0';


		if(!strncmp(buff,"NBR",3)){
			/* .... CALCUL du cli ext */
			int i = atoi(buff+3);
			char* res = execClientExec(cli,i);
			/* On récupere le resultat du cli ext, on l'envoie au serveur (et on s'assure de la bonne reception en option) */
			if(res){
				sprintf(buff,"RES %d:%s\n", i, res);
				w=write(serverSocket,buff,strlen(buff));
				if(w==-1){
					perror("Erreur ecriture");
					exit(1);
				}
			}else{
				sprintf(buff,"RES %d:#\n",i);
				w=write(serverSocket,buff,strlen(buff));
				if(w==-1){
					perror("Erreur ecriture");
					exit(1);
				}
			}
		}else{
			/* Erreur retour GET */
			/* Attendre un certain temps */
		}


	}

	printf("DCX\n");
	fgets(buff,MAX_LENGTH-1,stdin);
	if(!strncmp(buff,"DOK",3)){
		printf("Fin du client\n");
	}else{
		printf("Erreur coté serveur\n");
	}

	/* Netoyage memoire */


	return 0;
}