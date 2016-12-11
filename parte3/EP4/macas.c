/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: 8991769
    Raphael dos Reis Gusmao     NUSP: 9778561
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "asm.h"
#include "error.h"

const char out_suff[] = ".maco";

/*
  Dado o nome do arquivo de entrada gera
  o nome do arquivo de saída
*/

char* outfile_name(char *name) {
	int s, i = 0, tam;
	char *out;
	tam = strlen(name);
	out = malloc(sizeof(char)*(tam + 10));	
	while(i < tam && name[i] != '.') {
		out[i] = name[i];
		i++;
	}
	s = i;
	tam = strlen(out_suff);
	for(i = s; i < (s + tam); i++)
		out[i] = out_suff[i - s];
	
	return out;
}


int main(int argc, char *argv[]) {
	int i;
	FILE *in, *out;
	char *infile, *outfile;
	
	for(i = 1; i < argc; i++) {
		infile = argv[i];
		outfile = outfile_name(infile);
		in = fopen(infile, "r");
		out = fopen(outfile, "w");
		if(!assemble(infile, in, out)) 
			die("error");
	}
	return 0;
}
