#include "client.h"


static int thread_continu;

void *thread_ping(void *arg){
    /* Pour enlever le warning */
	/*(void) arg;*/
	DATA* d = (DATA*)arg; 

	int socketServeur;

	socketServeur=socket(AF_INET,SOCK_DGRAM,0);
	if(socketServeur<0){
		perror("error socket");
		exit(1);
	}


	struct sockaddr_in addr_r;
	memset(&addr_r,0,sizeof(addr_r));
	addr_r.sin_family=AF_INET;
	addr_r.sin_port=htons(PORT_MULTICAST);
	addr_r.sin_addr.s_addr=htonl(INADDR_ANY);

	struct ip_mreq imr;
	imr.imr_multiaddr.s_addr=inet_addr(GROUP_MULTICAST);
	imr.imr_interface.s_addr=INADDR_ANY;

	if(setsockopt(socketServeur,IPPROTO_IP,IP_ADD_MEMBERSHIP,&imr,sizeof(imr))<0){
		perror("settruc err");
		exit(1);
	}

	unsigned long on=1;
	if(setsockopt(socketServeur,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(imr))<0){
		perror("settruc err reuse");
		exit(1);
	}

	if(bind(socketServeur,(struct sockaddr*)&addr_r,sizeof(addr_r))<0){
		perror("err bind");
		exit(1);
	}

	int cnt;
	socklen_t len_r;
	char buff[100];


    /* Boucle d'attente des PING */
	while(thread_continu){
		len_r=sizeof(addr_r);
		printf("attente ping\n");
		cnt=recvfrom(socketServeur,buff,sizeof(buff),0,(struct sockaddr*)&addr_r,&len_r);
		if(cnt<0){
			perror("error");
			exit(1);
		}

		if(!strncmp(buff,"PIN",3)){
			int w=write(d->socket,"PON",3);
			if(w==-1){
				perror("Erreur ecriture");
				exit(1);
			}

			/* VERSION UDP */
			/*
			printf("PING recu\n");
			sprintf(buff,"PON");
			cnt=sendto(socketServeur,buff,strlen(buff),0,(struct sockaddr*)&addr_r,len_r);
			if(cnt!=strlen(buff)){
				printf("erreur thread sendto\n");
			}
			*/
		}else{
			printf("ERREUR thread recu : %s\n",buff );
		}

	}


	pthread_exit(NULL);
} 





int main(int argc, char* argv[]){
	printf("Lancement du client ...\n");

	ClientExec* cli = newClientExec();
	int error;
	int continu=1;
	int r,w,s;
	char buff[MAX_LENGTH];

	int numb=10;
	int verbose_flag=0;

	/* Creation du thread pour les PING */
	pthread_t thread;
	thread_continu=1;
	DATA* data;

	


	/* Recuperation des parametres */
	int c;
	while(1){
		struct option long_options[] ={
			{"verbose",		no_argument,       0, 'v'},
			{"numb",  	required_argument, 0, 'n'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		c = getopt_long (argc, argv, "n:v",long_options, &option_index);
		if (c == -1)
			break;
		switch (c){
			case 'n':
			numb = atoi(optarg);
			break;
			case 'v':
			verbose_flag++;
			break;
			default:
			abort ();
		}
	}





	/* Initialiser sockets, ... */


	int serverSocket;
	struct addrinfo hints, *rp;
	struct sockaddr_in6  serv_addr;

	memset ( (char *) &hints, 0, sizeof(serv_addr) );
	/* Compatibilite IPv6 */

	struct addrinfo *result;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	s = getaddrinfo(argv[1], argv[2], &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}


	/***********************/
	/*  Création du socket */
	/***********************/
	 for (rp = result; rp != NULL; rp = rp->ai_next) {
        serverSocket = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
		if (serverSocket==-1){
			continue;
		}
		error = connect (serverSocket, rp->ai_addr, rp->ai_addrlen );
		if(error !=-1 ){
			break;
		}else{
			perror ("erreur connect");
			exit (1);
		}
	}
	
	if (serverSocket <0) {
		perror ("erreur socket");
		exit (1);
	}
	if(rp==NULL){
		printf("ERROR\n");
		exit(1);
	}


	/* Connexion au serveur */
	/*error = connect (serverSocket, (struct sockaddr *) &serv_addr,  sizeof(serv_addr) );
	if(error < 0){
		perror ("erreur connect");
		exit (1);
	}*/



	/***********************/
	/* Lancement du thread */
	/***********************/
	
	data = (DATA*)xmalloc(sizeof(DATA)*1);
	data->socket=serverSocket;

	if(pthread_create(&thread, NULL, thread_ping, data) == -1) {
		perror("pthread_create");
		return EXIT_FAILURE;
	} 

	/* s'enregistrer sur le serveur, récuperer son ID */
	/* CNX */
	w=write(serverSocket,"CNX",3);
	if(w==-1){
		perror("Erreur ecriture");
		exit(1);
	}
	

	/* attente COK */
	r=read(serverSocket,buff,MAX_LENGTH-1);
	if(r==-1){
		perror("Erreur read");
		exit(1);
	}
	if(strncmp(buff,"COK",3)){
		printf("|%s|\n",buff );
		printf("Erreur de connexion\n");
		exit(1);
	}

	/* debut de boucle sur le programme externe */
	while(continu){

		/* on envoie un GET */
		w=write(serverSocket,"GET\n",4);
		if(w==-1){
			perror("Erreur ecriture");
			exit(1);
		}

		/* On recupere le nombre du serveur */
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
			}else{
				sprintf(buff,"RES %d:#\n",i);
			}
			w=write(serverSocket,buff,strlen(buff));
			if(w==-1){
				perror("Erreur ecriture");
				exit(1);
			}
			
		}else{
			/* Erreur retour GET */
			/* Attendre un certain temps */
			printf("ERREUR GET : |%s|\n",buff );
		}

		if(!--numb){
			continu=0;
		}
	}

	/* DCX */
	w=write(serverSocket,"DCX",3);
	if(w==-1){
		perror("Erreur ecriture");
		exit(1);
	}

	/* attente DOK */
	r=read(serverSocket,buff,MAX_LENGTH-1);
	if(r==-1){
		perror("Erreur read");
		exit(1);
	}
	if(!strncmp(buff,"DOK",3)){
		printf("Fin du client\n");
	}else{
		printf("Erreur coté serveur : |%s|\n",buff);
	}

	/* Netoyage memoire */
	thread_continu=0;
	deleteClienExec(cli);
	close(serverSocket);


	return 0;
}