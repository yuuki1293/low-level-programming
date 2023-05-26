CC = gcc
LD = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Werror -Wextra -ggdb -std=c89

all: main

main: main.o list.o higher_order.o file.o
	$(LD) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o main

.PHONY: clean