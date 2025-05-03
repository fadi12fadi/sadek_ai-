CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I./include
LDFLAGS = -lsqlite3

SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

# Créer les répertoires s'ils n'existent pas
$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR))

# Sources et objets
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Nom de l'exécutable
TARGET = $(BIN_DIR)/thesis_manager

# Règle par défaut
all: $(TARGET)

# Règle pour l'exécutable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Règle pour les fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/$(TARGET)

# Exécution
run: all
	./$(TARGET)

.PHONY: all clean run 