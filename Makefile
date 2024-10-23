# Compiler and flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic -std=c89 -Iutils

# Directories
UTILS_DIR = utils
OBJ_DIR = obj
TEST_DIR = tests

# Target executable name
TARGET = assembler
TEST_TARGET = test_runner

# Source files
MAIN_SRC := main.c
SRC_FILES := $(filter-out $(MAIN_SRC), $(wildcard *.c))
UTILS_FILES := $(wildcard $(UTILS_DIR)/*.c)
TEST_FILES := $(wildcard $(TEST_DIR)/*.c)
SRCS := $(MAIN_SRC) $(SRC_FILES) $(UTILS_FILES)

# Object files for the main executable
MAIN_OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(MAIN_SRC))
APP_OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES)) \
            $(patsubst $(UTILS_DIR)/%.c,$(OBJ_DIR)/%.o,$(UTILS_FILES))
OBJS := $(APP_OBJS) $(MAIN_OBJS)

# Object files for tests (excluding main.c)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_FILES))
TEST_APP_OBJS := $(APP_OBJS)  # Reuse APP_OBJS without main.o

# Default target
all: $(TARGET)

# Link object files to create the main executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files in current directory to object files
$(OBJ_DIR)/%.o: %.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile source files from utils to object files
$(OBJ_DIR)/%.o: $(UTILS_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test files to object files
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build the test runner (exclude main.o)
test: $(TEST_OBJS) $(TEST_APP_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJS) $(TEST_APP_OBJS)

# Run tests
run-test: test
	./$(TEST_TARGET)

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET) $(TEST_TARGET)

# Phony targets
.PHONY: all clean test run-test
