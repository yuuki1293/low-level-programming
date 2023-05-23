CC = gcc
LD = gcc
CFLAGS = -ansi -pedantic-errors -Wall -Werror -ggdb -std=c89

all: main

main: main.o is_prime.o list.o scalar_product.o higher_order.o
	$(LD) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o main

.PHONY: clean