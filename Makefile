# Compiler and flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic -std=c89 -Iutils

# Directories
UTILS_DIR = utils
OBJ_DIR = obj
TESTS_DIR = tests

# Executables
MAIN_EXEC = assembler
TEST_EXEC = test_runner

# Source files
MAIN_SRC = main.c
UTILS_SRC = $(wildcard $(UTILS_DIR)/*.c)
TEST_SRC = $(wildcard $(TESTS_DIR)/test_*.c)

# Object files
MAIN_OBJS = $(OBJ_DIR)/main.o
UTILS_OBJS = $(patsubst $(UTILS_DIR)/%.c,$(OBJ_DIR)/%.o,$(UTILS_SRC))
TEST_OBJS = $(patsubst $(TESTS_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_SRC))

# Default target: Build the main executable
all: $(MAIN_EXEC)

# Build the main executable
$(MAIN_EXEC): $(MAIN_OBJS) $(UTILS_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Build the test executable
$(TEST_EXEC): $(TEST_OBJS) $(UTILS_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile main source file
$(OBJ_DIR)/main.o: $(MAIN_SRC)
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile utility source files
$(OBJ_DIR)/%.o: $(UTILS_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test source files
$(OBJ_DIR)/%.o: $(TESTS_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Run tests
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(MAIN_EXEC) $(TEST_EXEC)

.PHONY: all test clean
