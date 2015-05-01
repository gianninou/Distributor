#definition du compilateur
CC = gcc

DIR = 
#$(shell pwd)/

SRC = $(DIR)src/

DEST = $(DIR)bin/

#specification des options du compilateur
CFLAGS = -ansi -Wall -g

#librairies
LIB = -lm

#nom de l executable
PROG = Distributor
SERVER = Server
CLIENT = Client

#definition de la cible a reconstruire
all : directories $(DEST)$(PROG)

client : $(DEST)client.o $(DEST)clientExec.o $(DEST)utils.o $(DEST)generator.o $(DEST)customlinkedlist.o
	$(CC)  $(DEST)client.o $(DEST)clientExec.o $(DEST)utils.o $(DEST)generator.o $(DEST)customlinkedlist.o -o $(DEST)$(CLIENT) 


server : $(DEST)server.o $(DEST)utils.o $(DEST)generator.o $(DEST)customlinkedlist.o
	$(CC)  $(DEST)server.o $(DEST)utils.o $(DEST)generator.o $(DEST)customlinkedlist.o -o $(DEST)$(SERVER) 


$(DEST)$(PROG) : $(DEST)main.o  $(DEST)utils.o $(DEST)generator.o $(DEST)customlinkedlist.o
	$(CC) $(DEST)main.o  $(DEST)utils.o $(DEST)generator.o $(DEST)customlinkedlist.o -o $(DEST)$(PROG) 



$(DEST)main.o : $(SRC)main.c $(SRC)main.h
	$(CC) $(CFLAGS) -c $(SRC)main.c -o $(DEST)main.o

$(DEST)server.o : $(SRC)server.c $(SRC)server.h
	$(CC) $(CFLAGS) $(LIB) -c $(SRC)server.c -o $(DEST)server.o

$(DEST)client.o : $(SRC)client.c $(SRC)client.h
	$(CC) $(CFLAGS) $(LIB) -c $(SRC)client.c -o $(DEST)client.o

$(DEST)clientExec.o : $(SRC)clientExec.c $(SRC)clientExec.h
	$(CC) $(CFLAGS) $(LIB) -c $(SRC)clientExec.c -o $(DEST)clientExec.o

$(DEST)utils.o : $(SRC)utils.c $(SRC)utils.h
	$(CC) $(CFLAGS) $(LIB) -c $(SRC)utils.c -o $(DEST)utils.o

$(DEST)generator.o : $(SRC)generator.c $(SRC)generator.h
	$(CC) $(CFLAGS) $(LIB) -c $(SRC)generator.c -o $(DEST)generator.o

$(DEST)customlinkedlist.o : $(SRC)customlinkedlist.c $(SRC)customlinkedlist.h
	$(CC) $(CFLAGS) $(LIB) -c $(SRC)customlinkedlist.c -o $(DEST)customlinkedlist.o

#########################################################################

#exemple edition de liens et production de l executable
#$(PROG) : fich.o fich2.o
#	$(CC) fich.o fich2.o  -o $(PROG)


#exemple compilation du fichier fich
#$(DEST)fich.o : $(SRC)fich.c $(SRC)fich.h
#	$(CC) $(CFLAGS) $(LIB) -c $(SRC)fich.c -o $(DEST)fich.o

#########################################################################



#creation du dossier bin
directories:
	@mkdir -p $(DEST) 2>/dev/null

#destruction de tous les fichiers intermediaires crees
clean : 
	@rm -f $(DEST)*.o

#suppression de tout ce qui peut etre regenere
mrproper : clean
	@rm -f $(DEST)$(PROG)
	@rm -f $(DEST)$(CLIENT)
	@rm -f $(DEST)$(SERVER)
