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
BIN = $(EXE) $(RES:$(RES_DIR)/%=$(BIN_DIR)/%)
OBJDUMP = $(OBJ:$(OBJ_DIR)/%.o=$(BIN_DIR)/%.o.txt)

all: $(BIN)

run: $(BIN)
	cd $(BIN_DIR); \
	./main; \
	dotnet fsi combine.fsx

# 実行ファイル作成
$(EXE): $(OBJ)
	$(LD) -o $@ $^

# Cコンパイル
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

# ASMコンパイル
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	$(AS) $(ASFLAGS) -o $@ $<

# リソースファイルコピー
$(BIN_DIR)/%: $(RES_DIR)/%
	cp -f $< $(BIN_DIR)/

dec: $(OBJDUMP)

# デコンパイル
$(BIN_DIR)/%.o.txt: $(OBJ_DIR)/%.o
	objdump -D -M intel-mnemonic $< > $@

clean:
	rm -f obj/* bin/*

.PHONY: all run clean