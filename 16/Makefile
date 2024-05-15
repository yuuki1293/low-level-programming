AS = nasm
ASFLAGS = -f elf64 -g -F dwarf
CC = gcc
LD = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Werror -Wextra -ggdb -std=c89 -march=native
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
RES_DIR = res
EXE = $(BIN_DIR)/main
C_SRC = $(wildcard $(SRC_DIR)/*.c)
AS_SRC = $(wildcard $(SRC_DIR)/*.asm)
OBJ = $(C_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) $(AS_SRC:$(SRC_DIR)/%.asm=$(OBJ_DIR)/%.o)
RES = $(wildcard $(RES_DIR)/*)
BIN = $(EXE) $(RES:$(RES_DIR)/%=$(OBJ_DIR)/%)

all: $(BIN)

run: $(BIN)
	./bin/main

$(EXE): $(OBJ)
	$(LD) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	$(AS) $(ASFLAGS) -o $@ $<

$(OBJ_DIR)/%: $(RES_DIR)/%
	cp $(RES) $(BIN_DIR)/

clean:
	rm -f obj/* bin/*

.PHONY: all clean