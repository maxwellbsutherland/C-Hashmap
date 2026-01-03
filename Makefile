# Compiler
CC := gcc

# Flags
CFLAGS := -Wall -Wextra -std=c11 -Iinclude

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE_NAME := hashmap.exe

# Windows-friendly paths
OBJ_DIR_WIN := $(subst /,\,$(OBJ_DIR))
BIN_DIR_WIN := $(subst /,\,$(BIN_DIR))
TARGET := $(BIN_DIR)/$(EXE_NAME)
TARGET_WIN := $(subst /,\,$(TARGET))

# Sources and objects
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default target
all: $(TARGET)

# Link executable
$(TARGET): $(OBJS)
	if not exist "$(BIN_DIR_WIN)" mkdir "$(BIN_DIR_WIN)"
	$(CC) $(OBJS) -o $(TARGET)

# Compile objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	if not exist "$(OBJ_DIR_WIN)" mkdir "$(OBJ_DIR_WIN)"
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build
clean:
	if exist "$(OBJ_DIR_WIN)" rmdir /S /Q "$(OBJ_DIR_WIN)"
	if exist "$(BIN_DIR_WIN)" rmdir /S /Q "$(BIN_DIR_WIN)"

# Rebuild
rebuild: clean all

.PHONY: all clean rebuild