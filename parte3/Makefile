#
# Breno Helfstein Moura       NUSP: 9790972
# Lucas Daher                 NUSP: 8991769
# Raphael dos Reis Gusmao     NUSP: 9778561
#
.PHONY: clean
CC = gcc
CFLAGS = -Wall -std=c99 -O2 -g
OBJS = \
	parse_test.o \
	stable.o \
	parser.o \
	optable.o \
	error.o \
	buffer.o \
	asmtypes.o

all: parser

parser: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	make clean

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *~
