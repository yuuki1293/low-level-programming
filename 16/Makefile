AS = nasm
ASFLAGS = -f elf64 -g -F dwarf
CC = gcc
LD = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Werror -Wextra -ggdb -std=c89 -march=native

all: main

main: main.o bmp.o bmp_sepia.o image_sepia_c_example.o
	$(LD) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	rm -f *.o main

.PHONY: clean