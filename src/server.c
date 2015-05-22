#include "server.h"


void usage(void){
	printf("usage : ./server numero_port_serveur\n");
}

int main(int argc, char* argv[]){
	printf("Lancement du serveur ...\n");


    /* INITIALISATION */
    /* liste de client, Generator, sockets, listen, bind, ... */
	pthread_t thread;
	char* response = (char*)xmalloc(sizeof(char)*BUFF_LEN);
	ListRemoteClient* clients_list = listeRemote_init();
	Generator* gen = newGenerator();
	List* realocate = liste_init();

	int sockfd, nbfd, newsockfd;
	struct sockaddr_in  serv_addr, cli_addr;
    //int tab_fd[FD_SETSIZE];
	fd_set rset, pset;
	socklen_t clilen;

	int maxfdp1, sockcli, i, nrcv, nsnd;

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
    	pset=rset;
    	nbfd = select(maxfdp1, &pset, NULL, NULL, NULL);
    	if(FD_ISSET(sockfd, &pset)) {
    		clilen = sizeof(cli_addr);
    		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    		RemoteClient* rm = newRemoteClient(cli_addr, newsockfd);
    		if(listeRemote_get_size(clients_list) != MAX_CLIENTS) {
    			listeRemote_add_last(clients_list,rm);
    			FD_SET(newsockfd, &rset);
    			if(newsockfd >= maxfdp1) {
    				maxfdp1=newsockfd+1;
    			}
    			nbfd--;
    		}
    		listeRemote_print(clients_list);
    	}

    	i = 0;
    	char message[BUFF_LEN];
    	while((nbfd > 0) && (i <FD_SETSIZE)) {
        	
    		if((FD_ISSET(i, &pset))) {
    			sockcli = listeRemote_get_i_socket(clients_list, i);
    			if(sockcli>0){
    				memset(message, 0, BUFF_LEN);
	    			if ( (nrcv= read ( sockcli, message, sizeof(message)-1) ) < 0 )  {
	    				perror ("servmulti : : readn error on socket");
	    				exit (1);
	    			}
	    			memset(response, 0, BUFF_LEN);
	    			message[nrcv]='\0';
	    			int res=apdu(gen, realocate, message, response);
	    			if( res== 0) {
	    				close(sockcli);
	                    //tab_clients[i] = -1;
	    				FD_CLR(sockcli, &rset);
	    			} else if(res==1){
	    				if ( (nsnd = write (sockcli, response, strlen(response)) ) < 0 ) {
	    					printf ("servmulti : writen error on socket");
	    					exit (1);
	    				}
	    			}
	                //showGenerator(gen);
	    			nbfd--;
	    		}
    		}
    		i++;
    	}
    	i = 0;
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
			sleep(5);
		}
	}
}



int apdu(Generator* gen, List* liste, char* message, char* reponse){
	int res=1;
	if(!strncmp(message,"CNX",3)){
		//ajouter le client dans la liste
		sprintf(reponse,"COK");
	}else if(!strncmp(message,"GEN",3)){
		//generer un nombre
		int nb;
		if(liste->size>0){
			Elem* e = liste_get(liste,0);
			nb=e->number;
			liste_supprime(liste,e);
		}else{
			nb=getNumber(gen);	
		}
		sprintf(reponse,"NBR %d",nb);
	}else if(!strncmp(message,"RES",3)){
		//enregistrer la reponse
		int number=0;
		char* result=(char*)xmalloc(sizeof(char)*strlen(message));

		const char s[2] = ":";
		char *token;
		token = strtok(&message[3], s);
		number = atoi(token);
		token = strtok(NULL, s);
		strcpy(result,token);

		if(setResult(gen, number, result)){
			//sprintf(reponse,"ROK");
		}else{
			//sprintf(reponse,"RKO");
		}
		free(result);
		res=2;
	}else if(!strncmp(message,"PON",3)){
		//mettre à jour le timestamp
		res=2;
	}else if(!strncmp(message,"DNX",3)){
		//supprimer le client
		sprintf(reponse,"DOK");
		res=0;
	}else if(strlen(reponse)==0){
		printf("Un client à quitté inopinément\n");
		//suppimer le client
		res=0;
	}else{
		printf("Erreur message client reçu : |%s|\n",message );
		res=0;
	}
	return res;
}
