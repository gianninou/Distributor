CC = gcc

DIR = .
#$(shell pwd)/

SRC = $(DIR)/src

DEST = $(DIR)/bin

#specification des options du compilateur
CFLAGS = -Wall 
# -ansi -g

#librairies
LIBS = -lm -lpthread

#noms des executables
TEST = test
SERVER = server
CLIENT = client

SERVER_OBJECT_FILES = $(DEST)/server.o $(DEST)/remoteClient.o $(DEST)/utils.o $(DEST)/generator.o $(DEST)/customlinkedlist.o $(DEST)/listRemoteClient.o
CLIENT_OBJECT_FILES = $(DEST)/client.o $(DEST)/clientExec.o  $(DEST)/utils.o $(DEST)/generator.o $(DEST)/customlinkedlist.o
TEST_OBJECT_FILES = $(DEST)/test.o $(DEST)/remoteClient.o $(DEST)/utils.o $(DEST)/generator.o $(DEST)/customlinkedlist.o $(DEST)/listRemoteClient.o

all: $(DEST)/$(CLIENT) $(DEST)/$(SERVER) $(DEST)/$(TEST)

$(DEST)/$(CLIENT): $(CLIENT_OBJECT_FILES)
$(DEST)/$(SERVER): $(SERVER_OBJECT_FILES)
$(DEST)/$(TEST): $(TEST_OBJECT_FILES)

$(DEST)/$(SERVER) $(DEST)/$(CLIENT) $(DEST)/$(TEST):
	    @mkdir -p $(dir $@)
		    $(CC) $(CFLAGS) $^ $(LIBS) -o $@  

$(DEST)/%.o: $(SRC)/%.c
	    $(CC) -c $< -o $@

clean: 
	    rm -f $(DEST)/*.o $(DEST)/*~ 

mrproper: clean
		rm -f $(DEST)/$(SERVER) $(DEST)/$(CLIENT) $(DEST)/$(TEST) 

