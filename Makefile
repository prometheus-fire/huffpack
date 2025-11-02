# Define variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRC_DIR = src
BUILD_DIR = build

# 1. Definisci ESPLICITAMENTE i NOMI BASE dei programmi che vuoi come eseguibili.
# Questi nomi devono corrispondere ai tuoi file in src/ (es. src/main1.c, src/prog2.c).
PROGRAMS = huffpack huffunpack

# 2. Crea una lista degli eseguibili completi (es. main1 prog2)
EXEC = $(PROGRAMS)

# 3. Trova tutti i file sorgente C che non sono programmi (moduli di supporto)
# Questi sono tutti i file .c che non hanno il nome base in PROGRAMS.
SOURCES_MODULES = $(filter-out $(patsubst %, $(SRC_DIR)/%.c, $(PROGRAMS)), $(wildcard $(SRC_DIR)/*.c))

# 4. Lista dei file oggetto per i moduli (es. build/utils.o)
OBJECTS_MODULES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES_MODULES))

.PHONY: all clean rebuild

# --- TARGET PRINCIPALI ---

# Default target: crea la directory build, compila i moduli e poi gli eseguibili
all: $(BUILD_DIR) $(OBJECTS_MODULES) $(EXEC)

# --- REGOLE DI COMPILAZIONE ---

# Regola per la compilazione dei MODULI di supporto (.c -> .o)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling module $<..."
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