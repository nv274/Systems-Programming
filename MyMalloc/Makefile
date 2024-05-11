CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Target executables
TARGETS = memtestcase memtest memgrind

# Source files
SRCS = mymalloc.c memtestcase.c memtest.c memgrind.c

all: $(TARGETS)

memtestcase: mymalloc.c memtestcase.c
	$(CC) $(CFLAGS) $^ -o $@

memtest: mymalloc.c memtest.c
	$(CC) $(CFLAGS) $^ -o $@

memgrind: mymalloc.c memgrind.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS)