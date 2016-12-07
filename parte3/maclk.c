/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: ?
    Raphael dos Reis Gusmao     NUSP: 9778561
*/
#include<stdio.h>
#include<stlib.h>
#include "error.h"
#include "mactypes.h"
#include "asmtypes.h"
#include "optable.h"
#include "opcodes.h"


int main(int argc, char *argv[]) {
	int i;
	FILE *in, *out;
	char *infile, *execfile;
	exec = argv[argc - 1];
	for(i = 1; i < argc; i++) {
		infile = argv[i];
		outfile = exec_name(infile);
		in = fopen(infile, "r");
		out = fopen(outfile, "w");
		/*
		  Linka os arquivos e transforma em linguagem de
		  máquina.
		*/
	}
	return 0;
}

