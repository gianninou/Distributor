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

#definition de la cible a reconstruire
all : directories $(DEST)$(PROG)

$(DEST)$(PROG) : $(DEST)main.o $(DEST)utils.o
	$(CC) $(DEST)main.o $(DEST)utils.o -o $(DEST)$(PROG) 

$(DEST)main.o : $(SRC)main.c $(SRC)main.h
	$(CC) $(CFLAGS) -c $(SRC)main.c -o $(DEST)main.o

$(DEST)utils.o : $(SRC)utils.c $(SRC)utils.h
	$(CC) $(CFLAGS) $(LIB) -c $(SRC)utils.c -o $(DEST)utils.o


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
