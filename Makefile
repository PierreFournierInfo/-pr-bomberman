CC = gcc
CFLAGS = -Wall
LDFLAGS = -lncurses
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
RES_DIR = res

# Trouver tous les fichiers .c 
SRCS := $(shell find $(SRC_DIR) -name '*.c')

# Generer les fichiers objets
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Defintion du nom de l'executable
TARGET = $(BIN_DIR)/bomberman

# Cible par defaut
all: $(TARGET)

# Compilation des fichiers .c en .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Liaisons des fichiers objets a l'executable
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)
	cp -r $(RES_DIR) $(BIN_DIR)/

# Nettoyage des artefacts de compilation.
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run :
	$(BIN_DIR)/bomberman

br : all run

# Phony targets (not associated with files)
.PHONY: all clean
