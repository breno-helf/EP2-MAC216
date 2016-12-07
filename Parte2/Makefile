#
# Breno Helfstein Moura       NUSP: 9790972
# Lucas Daher                 NUSP: 8991769
# Raphael dos Reis Gusmao     NUSP: 9778561
#

CFLAGS = -Wall -std=c99 -O2 -g
OBJS = parse_test.o parser.o stable.o optable.o error.o buffer.o asmtypes.o # opcodes.o
.PHONY: clean

all: parser

parser: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	make clean

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o *~
