#
# Breno Helfstein Moura       NUSP: 9790972
# Lucas Daher                 NUSP: 8991769
# Raphael dos Reis Gusmao     NUSP: 9778561
#
.PHONY: clean
CC = gcc
CFLAGS = -Wall -std=c99 -O2 -g

all: macas maclk

macas: macas.o stable.o asm.o error.o mactypes.h asmtypes.o optable.o opcodes.h parser.o buffer.o
	$(CC) $(CFLAGS) $^ -o $@
	make clean

maclk: maclk.o stable.o error.o mactypes.h asmtypes.o optable.o opcodes.h parser.o buffer.o
	$(CC) $(CFLAGS) $^ -o $@
	make clean

parser: parse_test.o parser.o stable.o optable.o error.o buffer.o asmtypes.o
	$(CC) $(CFLAGS) $^ -o $@
	make clean

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *~
