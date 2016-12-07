#
# Breno Helfstein Moura       NUSP: 9790972
# Lucas Daher                 NUSP: 8991769
# Raphael dos Reis Gusmao     NUSP: 9778561
#

CFLAGS = -Wall -std=c99 -O2 -g
#Colocar todos os objetos a serem compilados
OBJS = stable.o parser.o optable.o error.o buffer.o asmtypes.o

.PHONY: clean

all:



%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o *~

