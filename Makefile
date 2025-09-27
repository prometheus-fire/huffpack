# Define variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC_DIR = src
BUILD_DIR = build
EXEC = test

# Find all C source files in the source directory
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Create a list of corresponding object files in the build directory
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

.PHONY: all clean rebuild

# Default target: builds the main executable
all: $(BUILD_DIR) $(EXEC)

# Rule to link the final executable
$(EXEC): $(OBJECTS)
	@echo "Linking executable..."
	$(CC) $(CFLAGS) $^ -o $@

# Rule to compile C source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create the build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Clean target: removes the executable and the build directory
clean:
	@echo "Cleaning up..."
	@rm -f $(EXEC)
	@rm -rf $(BUILD_DIR)

# Rebuild target: performs a clean followed by a fresh build
rebuild: clean all
