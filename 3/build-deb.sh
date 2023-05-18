#!/bin/bash
nasm -felf64 -g -F dwarf forth.asm -o forth.o
ld forth.o lib.o -o forth
