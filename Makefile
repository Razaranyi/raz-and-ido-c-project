# Compiler and flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic -std=c89 -Iutils

# Directories
SRC_DIR = src
TESTS_DIR = tests
OBJ_DIR = obj
UTILS_DIR = utils

# Source files
COMMON_SRC = $(wildcard $(UTILS_DIR)/*.c)

# Object files for the source code (common objects)
COMMON_OBJS = $(patsubst $(UTILS_DIR)/%.c, $(OBJ_DIR)/%.o, $(COMMON_SRC))

# Default target for compiling source files only
all: compile_sources

# Compile all source files (only compile, no linking)
compile_sources: $(COMMON_OBJS)
	@echo "Source files compiled successfully."

# Test target
TEST_EXEC = test_runner

# User-defined input for test name
TEST_NAME ?= all

# Test source dynamically determined by the input
TEST_SRC = $(wildcard $(TESTS_DIR)/test_$(TEST_NAME).c)

# Object files for the test
TEST_OBJ = $(patsubst $(TESTS_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_SRC))

# Default test target
test: $(TEST_EXEC)
	./$(TEST_EXEC)

# Build test runner based on the input
$(TEST_EXEC): $(TEST_OBJ) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile test sources into object files
$(OBJ_DIR)/%.o: $(TESTS_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile common object files
$(OBJ_DIR)/%.o: $(UTILS_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR)/*.o $(TEST_EXEC)

# Phony targets
.PHONY: test clean compile_sources all
