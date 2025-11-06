# Define variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRC_DIR = src
BUILD_DIR = build
C_WALK_DIR = $(SRC_DIR)/cwalk

# Aggiunge src/cwalk alle directory di inclusione
CFLAGS += -I$(C_WALK_DIR)

# 1. Definisci ESPLICITAMENTE i NOMI BASE dei programmi che vuoi come eseguibili.
# Questi nomi devono corrispondere ai tuoi file in src/ (es. src/main1.c, src/prog2.c).
PROGRAMS = huffpack huffunpack

# 2. Crea una lista degli eseguibili completi (es. huffpack huffunpack)
EXEC = $(PROGRAMS)

# 3. Definisci i file sorgente per i moduli di supporto (non eseguibili)
# Sorgenti generici in src/
SOURCES_GENERIC = $(filter-out $(patsubst %, $(SRC_DIR)/%.c, $(PROGRAMS)), $(wildcard $(SRC_DIR)/*.c))

# Sorgenti specifici della libreria cwalk in src/cwalk/
SOURCES_C_WALK = $(wildcard $(C_WALK_DIR)/*.c)

# Unisci tutti i sorgenti dei moduli
SOURCES_MODULES = $(SOURCES_GENERIC) $(SOURCES_C_WALK)

# 4. Lista dei file oggetto per i moduli
# Trasforma i path dei sorgenti in path di oggetti nella cartella BUILD_DIR
OBJECTS_GENERIC = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES_GENERIC))
OBJECTS_C_WALK = $(patsubst $(C_WALK_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES_C_WALK))

# Unisci tutti gli oggetti dei moduli
OBJECTS_MODULES = $(OBJECTS_GENERIC) $(OBJECTS_C_WALK)

.PHONY: all clean rebuild

# --- TARGET PRINCIPALI ---

# Default target: crea la directory build, compila i moduli e poi gli eseguibili
all: $(BUILD_DIR) $(OBJECTS_MODULES) $(EXEC)

# --- REGOLE DI COMPILAZIONE ---

# Regola per la compilazione dei MODULI generici (.c in src/ -> .o in build/)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling module $<..."
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Regola per la compilazione dei MODULI cwalk (.c in src/cwalk/ -> .o in build/)
$(BUILD_DIR)/%.o: $(C_WALK_DIR)/%.c
	@echo "Compiling cwalk module $<..."
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Regola per la compilazione e il linking di OGNI ESEGUIBILE
# L'eseguibile dipende dal suo file .c e da TUTTI i file oggetto dei moduli
%: $(SRC_DIR)/%.c $(OBJECTS_MODULES)
	@echo "Linking executable $@..."
	$(CC) $(CFLAGS) $^ -o $@

# Regola per creare la build directory se non esiste
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# --- PULIZIA ---

# Clean target: rimuove TUTTI gli eseguibili, gli oggetti e la directory build
clean:
	@echo "Cleaning up..."
	@rm -f $(EXEC)
	@rm -rf $(BUILD_DIR)

# Rebuild target: pulisce e ricompila tutto
rebuild: clean all