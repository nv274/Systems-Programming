CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11

.PHONY: all clean

all: spchk

spchk: spchk.o
	$(CC) $(CFLAGS) -o $@ $^

spchk.o: spchk.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f spchk spchk.o
