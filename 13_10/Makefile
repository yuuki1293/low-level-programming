CC = gcc
LD = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Werror -Wextra -ggdb -std=c89

all: main

main: main.o bmp.o
	$(LD) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o main

.PHONY: clean