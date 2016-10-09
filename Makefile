CC=gcc
CFLAGS=-Wall -std=c99 -O2 -g

all: freq

freq: main.c stable.o error.o buffer.o
	$(CC) $(CFLAGS) $^ -o $@
stable.o: stable.c stable.h
	$(CC) $(CFLAGS) -c $< -o $@
buffer.o: buffer.c buffer.h
	$(CC) $(CFLAGS) -c $< -o $@
error.o: error.c error.h
	$(CC) $(CFLAGS) -c $< -o $@
