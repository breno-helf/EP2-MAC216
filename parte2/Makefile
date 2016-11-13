#
# Breno Helfstein Moura       NUSP: 9790972
# Lucas Daher                 NUSP: ?
# Raphael dos Reis Gusmao     NUSP: 9778561
#

CFLAGS = -Wall -std=c99 -O2 


.PHONY: clean


all: parser


parser: parser.o stable.o error.o buffer.o asmtypes.o opcodes.o parse_test.o
	$(CC) -o $@ $^


%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f *.o *~ center freq
