#include "server.h"


void usage(void){
	printf("usage : ./server numero_port_serveur\n");
}

int main(int argc, char* argv[]){
	printf("Lancement du serveur ...\n");


    /* INITIALISATION */
    /* liste de client, Generator, sockets, listen, bind, ... */
	pthread_t thread;
	char* reponse = (char*)xmalloc(sizeof(char)*BUFF_LEN);
	ListRemoteClient* clients_list = listeRemote_init();
	Generator* gen = newGenerator();

    int sockfd;//, nbfd, newsockfd;
    struct sockaddr_in  serv_addr, cli_addr;
    //int tab_fd[FD_SETSIZE];
    fd_set rset, pset;
    socklen_t clilen;

    int maxfdp1;

    /* Verifier le nombre de paramètre en entrée */
    if (argc != 2){
    	usage();
    	exit(1);
    }

    /*
    * Ouvrir une socket (a TCP socket)
    */
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) <0) {
    	perror("servmulti : Problème socket\n");
    	exit(2);
    }

    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    /*
    * Lier l'adresse locale�à la socket
    */
    memset( (char*) &serv_addr,0, sizeof(serv_addr) );
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr) ) <0) {
    	perror ("servmulti : erreur bind\n");
    	exit (1);
    }
    /* Paramétrer le nombre de connexion "pending" */
    if (listen(sockfd, SOMAXCONN) <0) {
    	perror ("servmulti : erreur listen\n");
    	exit (1);
    }

    maxfdp1 = sockfd + 1;
    FD_ZERO(&rset);
    FD_ZERO(&pset);
    FD_SET(sockfd, &rset);


    /* Lancement du thread */
    if(pthread_create(&thread, NULL, thread_ping, NULL) == -1) {
    	perror("pthread_create");
    	return EXIT_FAILURE;
    } 


    /* boucle attente client */

    for(;;) {
    	pset = rset;

        //nbfd =
    	select(maxfdp1, &pset, NULL, NULL, NULL);

    	if(FD_ISSET(sockfd, &pset)) {
    		clilen = sizeof(cli_addr);
            //newsockfd = 
    		accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    		RemoteClient* rm = newRemoteClient(cli_addr);
    		if(listeRemote_get_size(clients_list) != MAX_CLIENTS) {
    			listeRemote_add_last(clients_list,rm);
    		}
    	}
    	listeRemote_print(clients_list);
    	printf("\n\n");
    }
    listeRemote_dest(clients_list);
    close(sockfd);
    /* creer un thread pour PING les clients avec gestion des pertes de connexion*/

    /* Si client déjà enregistré, alors repondre à ces demandes */

    return 0;
}

void *thread_ping(void *arg){
	(void) arg;
	
	int serverSocket, n;
	struct sockaddr_in  serv_addr;
	char *data="PIN";
	int ttl;  
	
	memset( (char *) &serv_addr,0, sizeof(serv_addr) );
	serv_addr.sin_family = PF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(GROUP_MULTICAST);
	serv_addr.sin_port = htons(PORT_MULTICAST);

	if ((serverSocket = socket(PF_INET, SOCK_DGRAM, 0)) <0) {
		perror ("erreur socket thread");
		exit (1);
	}

	ttl=1;
	if (setsockopt(serverSocket, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) <0){
		perror ("setsockopt thread");
		exit (1);
	}

	while(1){
		n= sendto (serverSocket, data, strlen(data),0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
		if (n != strlen(data)){
			perror ("erreur sendto thread");
			/*exit (1);*/
		}else{
			printf("PINGGGG\n");
			sleep(1);
		}
	}
}



int adpu(Generator* gen, char* message, char* reponse){
	int res=1;
	if(!strncmp(message,"CNX",3)){
		//ajouter le client dans la liste
		sprintf(reponse,"COK");
	}else if(!strncmp(message,"GEN",3)){
		//generer un nombre
		int nb=getNumber(gen);
		sprintf(reponse,"NBR %d",nb);
	}else if(!strncmp(message,"RES",3)){
		//enregistrer la reponse
		int number=0;
		char* result=(char*)xmalloc(sizeof(char)*strlen(message));
		sscanf(&message[4],"%d:%s",&number,result);
		free(result);
		if(setResult(gen, number, result)){
			sprintf(reponse,"ROK");
		}else{
			sprintf(reponse,"RKO");
		}
	}else if(!strncmp(message,"PON",3)){
		//mettre à jour le timestamp
	}else if(!strncmp(message,"DNX",3)){
		//supprimer le client
		sprintf(reponse,"DOK");
	}else{
		printf("Erreur message client reçu : |%s|\n",message );
		res=0;
	}
	return res;
}
