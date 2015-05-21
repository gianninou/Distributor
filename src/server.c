#include "server.h"


void usage(void){
  printf("usage : ./server numero_port_serveur\n");
}

int main(int argc, char* argv[]){
     printf("Lancement du serveur ...\n");


        /* INITIALISATION */
        /* liste de client, Generator, sockets, listen, bind, ... */

    ListRemoteClient* clients_list = listeRemote_init();

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
