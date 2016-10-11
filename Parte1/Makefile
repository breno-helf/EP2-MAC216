#
# Breno Helfstein Moura       NUSP: 9790972
# Lucas Daher                 NUSP: ?
# Raphael dos Reis Gusmao     NUSP: 9778561
#

CFLAGS = -Wall -std=c99 -O2 


.PHONY: clean


all: center freq


center: center.o stable.o error.o buffer.o
	$(CC) -o $@ $^


freq: freq.c stable.o error.o buffer.o
	$(CC) -o $@ $^


%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f *.o *~ center freq
