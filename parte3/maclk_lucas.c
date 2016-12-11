/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: 8991769
    Raphael dos Reis Gusmao     NUSP: 9778561
*/
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "mactypes.h"
#include "asmtypes.h"
#include "optable.h"
#include "opcodes.h"
#include "stable.h"
#include "parser.h"
#include "buffer.h"

/* ./maclk teste.mac abc1.maco abc2.maco */
int main (int argc, char *argv[]) {
	int i, line,  num_lines, j, k, l;
	FILE *in, *out;
	char *infile, *execfile, *buffer_aux;
	Buffer *buffer;
	buffer_aux = malloc (sizeof (char) * 10);
	SymbolTable extern_table;
	InsertionResult res;
	extern_table = stable_create();
	execfile = argv[1];
	out = fopen(execfile, "w");

	line = num_lines = 0;
	for (i = 2; i < argc; i++) {
		infile = argv[i];
		in = fopen(infile, "r");
		
		read_line(in, buffer);
		l = atoi(buffer->data);
		read_line(in, buffer);
		while (buffer->data[0] == 'E') {
			k = 0;
			for (j = 2; buffer->data[j] != ' '; j++) {
				buffer_aux[k] = buffer->data[j];
				k++;
			}
			buffer_aux[k] = '\0';
			if (stable_find(extern_table, buffer_aux) != NULL)
				die("%s label already defined\n", buffer_aux);
			res = stable_insert(extern_table, buffer_aux);
			res.data->i = atoi(&(buffer->data[j + 1])) + num_lines;
		}
		num_lines += l;
		while (read_line (in, buffer) != -1);
	}
	if (stable_find(extern_table, "main") == NULL)
		die("main not defined\n");
	rewind(in);
	i = line = 0;
	while (read_line(in, buffer) != -1) {
		i = atoi(buffer->data);
		while (buffer->data[0] != 'B')
			read_line(in, buffer);
		read_line(in, buffer);
		for (j = 0; j < i; j++) {
			if (buffer->data[0] == '*') {
				res.data = stable_find (extern_table, &(buffer->data[5]));
				if (res.data == NULL)
					die("%s not defined\n", &(buffer->data[5]));
				k = res.data->i - line;
				if (k < 0)
					fprintf(out, "49%06x\n", -k);
				else
					fprintf(out, "48%06x\n", k);
			}
			else
				fprintf(out, "%s", buffer->data);
			read_line(in, buffer);
			line ++;
		}
	}
	return 0;
}
