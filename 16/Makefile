AS = nasm # ASMコンパイラ
ASFLAGS = -f elf64 -g -F dwarf # ASMコンパイラフラク
CC = gcc # Cコンパイラ
LD = gcc # リンカ
CFLAGS = -ansi -pedantic-errors -Wall -Werror -Wextra -ggdb -std=c89 -march=native # Cコンパイラフラグ
SRC_DIR = src # ソースディレクトリ
OBJ_DIR = obj # オブジェクトディレクトリ
BIN_DIR = bin # バイナリディレクトリ
RES_DIR = res # リソースディレクトリ
EXE = $(BIN_DIR)/main # 実行ファイル
C_SRC = $(wildcard $(SRC_DIR)/*.c)  # .cファイルのリスト
AS_SRC = $(wildcard $(SRC_DIR)/*.asm) # .asmファイルのリスト
OBJ = $(C_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) $(AS_SRC:$(SRC_DIR)/%.asm=$(OBJ_DIR)/%.o) # .oファイルのリスト
RES = $(wildcard $(RES_DIR)/*) # リソースファイルのリスト
BIN = $(EXE) $(RES:$(RES_DIR)/%=$(BIN_DIR)/%) # 実行ファイルとリソースファイルのリスト

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

clean:
	rm -f obj/* bin/*

.PHONY: all run clean