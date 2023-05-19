#!/bin/bash
nasm -felf64 -g -F dwarf forth.asm -o forth.o
nasm -felf64 lib.asm -o lib.o
ld forth.o lib.o -o forth

nasm -felf64 -E forth.asm -o forth_E