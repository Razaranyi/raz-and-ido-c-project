# Compiler and flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic -std=c89

# Directories
TESTS_DIR = tests
OBJ_DIR = obj
UTILS_DIR = utils
CORE_DIR = core
EXEC_DIR = exec
# Source files
COMMON_SRC = $(wildcard $(UTILS_DIR)/*.c) $(wildcard $(CORE_DIR)/*.c) $(wildcard $(EXEC_DIR)/*.c)

# Object files for the source code (excluding tests and main.c for tests)
COMMON_OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(COMMON_SRC)))

# Target for building the assembler executable
assembler: $(COMMON_OBJS) $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) -o assembler $(COMMON_OBJS) $(OBJ_DIR)/main.o
	@echo "Executable 'assembler' created successfully."

# Compile test sources into object files
$(OBJ_DIR)/%.o: $(TESTS_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile common object files
$(OBJ_DIR)/%.o: $(UTILS_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile core object files
$(OBJ_DIR)/%.o: $(CORE_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(EXEC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile main object file for the assembler executable only
$(OBJ_DIR)/main.o: main.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR)/*.o $(TEST_EXEC) assembler

