#include "server.h"


void usage(void){
	printf("usage : ./server numero_port_serveur\n");
}

int main(int argc, char* argv[]){
	printf("Lancement du serveur ...\n");


    /* INITIALISATION */
    /* liste de client, Generator, sockets, listen, bind, ... */
	int time_stamp;
	pthread_t thread;
	char* response = (char*)xmalloc(sizeof(char)*BUFF_LEN);
	ListRemoteClient* clients_list = listeRemote_init();
	Generator* gen = newGenerator();
	List* realocate = liste_init();
	char str[1000];

	int sockfd, nbfd, newsockfd;
	struct sockaddr_in cli_addr;
	struct addrinfo hints, *rp;
    //int tab_fd[FD_SETSIZE];
	fd_set rset, pset;
	socklen_t clilen;

	int maxfdp1=0, sockcli, i, nrcv, nsnd, s;

    /* Verifier le nombre de paramètre en entrée */
	if (argc != 2){
		usage();
		exit(1);
	}

    /*
    * Ouvrir une socket (a TCP socket)
    */
    /*if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) <0) {
    	perror("servmulti : Problème socket\n");
    	exit(2);
    }*/



    /*
    * Lier l'adresse locale�à la socket
    */
    struct addrinfo *result;
    memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = (AI_PASSIVE);
    hints.ai_protocol = 0;

    s = getaddrinfo(NULL, argv[1], &hints, &result);
    if (s != 0) {
    	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    	exit(EXIT_FAILURE);
    }

	/***********************/
	/*  Création du socket */
	/***********************/
	int tab_fd[10];
	memset(tab_fd,0,10);
	int maxfd=0;
	FD_ZERO(&rset);
	FD_ZERO(&pset);
    for (rp = result; rp != NULL; rp = rp->ai_next) {
    	sockfd = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
    	if (sockfd!=-1){
    		int optval = 1;
    		setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    		if (bind(sockfd, rp->ai_addr, rp->ai_addrlen ) <0) {
    			perror ("servmulti : erreur bind\n");
    			exit (1);
    		}
    		if (sockfd <0) {
		    	perror ("erreur socket");
		    	exit (1);
		    }
		    // if(rp==NULL){
		    // 	printf("ERROR\n");
		    // 	exit(1);
		    // }

		    /*memset( (char*) &serv_addr,0, sizeof(serv_addr) );
		    serv_addr.sin_family = PF_INET;
		    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		    serv_addr.sin_port = htons(atoi(argv[1]));*/

		    /* Paramétrer le nombre de connexion "pending" */
		    if (listen(sockfd, SOMAXCONN) <0) {
		    	perror ("servmulti : erreur listen\n");
		    	exit (1);
		    }
		    maxfdp1 = sockfd + 1;
			
			FD_SET(sockfd, &rset);
			if(maxfd<10){
		    	tab_fd[maxfd]=sockfd;
		    	maxfd++;
			}else{
				break;
			}
		    
		}
    }






    /* Lancement du thread */
    if(pthread_create(&thread, NULL, thread_ping, NULL) == -1) {
    	perror("pthread_create");
    	return EXIT_FAILURE;
    }

    /* boucle attente client */
    //time_stamp=(unsigned)time(NULL);
    for(;;) {
    	pset=rset;
    	nbfd = select(maxfdp1, &pset, NULL, NULL, NULL);
    	int ii=0;
    	for(ii=0;ii<maxfd;ii++){
	    	if(FD_ISSET(tab_fd[ii], &pset)) {
	    		clilen = sizeof(cli_addr);
	    		newsockfd = accept(tab_fd[ii], (struct sockaddr *)&cli_addr, &clilen);
	    		RemoteClient* rm = newRemoteClient(cli_addr, newsockfd);
	    		if(listeRemote_get_size(clients_list) != MAX_CLIENTS) {
	    			listeRemote_add_last(clients_list,rm);
	    			//printRemoteClient(rm);
	    			FD_SET(newsockfd, &rset);
	    			if(newsockfd >= maxfdp1) {
	    				maxfdp1=newsockfd+1;
	    			}
	    			nbfd--;
	    		}
	    		//listeRemote_print(clients_list);
	    	}
	    }

    	i = 0;
    	char message[BUFF_LEN];
    	while((nbfd > 0) && (i <FD_SETSIZE)) {
    		if((FD_ISSET(i, &pset))) {
    			RemoteClient* c = listeRemote_get_i_socket(clients_list, i);
    			sockcli = c->dialog_socket;
    			if(sockcli>0){
    				memset(message, 0, BUFF_LEN);
    				if ( (nrcv= read ( sockcli, message, sizeof(message)-1) ) < 0 )  {
    					perror ("servmulti : : readn error on socket");
    					exit (1);
    				}
    				memset(response, 0, BUFF_LEN);
    				message[nrcv]='\0';
    				strcpy(str, message);
    				char* token = strtok(str, "|");
    				int truc=0;
    				while( token != NULL || strlen(message)==0) {
    					truc++;
						
    					int res=apdu(gen, realocate,c, token, response);
    					if(res== 0 || res==3) {

    						if(res==3){
    							liste_add_last(realocate,c->number,NULL);
    						}
    						listeRemote_suppr_i_socket(clients_list, c->dialog_socket);
    						close(sockcli);
    						FD_CLR(sockcli, &rset);
    						break;
							//listeRemote_print(clients_list);
    					} else if(res==1){
    						if ( (nsnd = write (sockcli, response, strlen(response)) ) < 0 ) {
    							printf ("servmulti : writen error on socket");
    							exit (1);
    						}
    					}
    					token = strtok(NULL, "|");
    				}
    					
	                //showGenerator(gen);
    				nbfd--;
    			}
    		}
    		i++;
    	}
    	i = 0;

    	int t2 = (unsigned)time(NULL);
    	if(t2 - time_stamp >10){
    		ElemRemoteClient* elrm = listeRemote_tete(clients_list);
    		ElemRemoteClient* elrm2=elrm;
    		while(elrm2!=NULL){
    			elrm2=elrm;
    			if(t2 - elrm2->remoteClient->timestamp_last_pong_sent > 10){
    				printf("KILL %d\n",elrm->remoteClient->id );
    				liste_add_last(realocate,elrm->remoteClient->number,NULL);
    				close(elrm->remoteClient->dialog_socket);
    				FD_CLR(elrm->remoteClient->dialog_socket, &rset);
    				listeRemote_suppr_i_socket(clients_list, elrm->remoteClient->dialog_socket);
    			}
    			if(elrm2->suivant==NULL){
    				break;
    			}else{
    				elrm=elrm2->suivant;
    			}
    			
    		}
    		time_stamp=(unsigned)time(NULL);
    	}



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
			sleep(8);

		}
	}
}



int apdu(Generator* gen, List* liste,RemoteClient* c, char* message, char* reponse){
	int res=1;
	if(message!=NULL){
		if(!strncmp(message,"CNX",3)){
			//ajouter le client dans la liste
			sprintf(reponse,"COK");
		}else if(!strncmp(message,"GEN",3)){
			//generer un nombre
			int nb;
			if(liste->size>0){
				Elem* e = liste->tete;
				nb=e->number;
				liste_supprime(liste,e);
			}else{
				nb=getNumber(gen);
			}
			c->number=nb;
			sprintf(reponse,"NBR %d",nb);
		}else if(!strncmp(message,"RES",3)){
			//enregistrer la reponse
			int number=0;
			char* result=(char*)xmalloc(sizeof(char)*strlen(message));
			char str[1000];
			int i;
			for(i=3;i<strlen(message);i++){
				if(message[i]==':'){
					break;
				}
			}
			strcpy(str,message);
			number = atoi(&message[3]);
			strcpy(result,&message[i+1]);

			if(setResult(gen, number, result)){
				//sprintf(reponse,"ROK");
			}else{
				//sprintf(reponse,"RKO");
			}
			free(result);
			res=2;
		}else if(!strncmp(message,"PON",3)){
			c->timestamp_last_pong_sent=(unsigned)time(NULL);
			res=2;
		}else if(!strncmp(message,"DNX",3)){
			//supprimer le client
			sprintf(reponse,"DOK");
			res=0;
		}else if(strlen(reponse)==0){
			printf("Un client a quitté inopinément\n");
			res=3;
		}else{
			printf("Erreur message client reçu : |%s|\n",message );
			res=0;
		}
	}else{
		printf("Un client est parti (CTRL-C)\n");
		res=3;
	}
	return res;
}
