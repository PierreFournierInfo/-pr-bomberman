CC = gcc
CFLAGS = -Wall
LDFLAGS = -lncurses
BIN_DIR = bin
SRC_DIR = src
CURR_DIR = $(shell pwd)

# Defintion du nom de l'executable
CLIENT_TARGET = $(BIN_DIR)/bomberman_client

SERVER_TARGET = $(BIN_DIR)/bomberman_server

# Cible par defaut
all: $(CLIENT_TARGET)

client : $(CLIENT_TARGET)

server: $(SERVER_TARGET)

$(CLIENT_TARGET):
	mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $(CLIENT_TARGET) $(SRC_DIR)/bomberman_client.c

$(SERVER_TARGET):
	mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $(SERVER_TARGET) $(SRC_DIR)/bomberman_server.c

# Nettoyage des artefacts de compilation.
clean:
	rm -rf $(BIN_DIR)

rclient :
	xterm -e $(CURR_DIR)/$(BIN_DIR)/bomberman_client &

rserver :
	xterm -e $(CURR_DIR)/$(BIN_DIR)/bomberman_server &

br : all run

# Phony targets (not associated with files)
.PHONY: all clean
