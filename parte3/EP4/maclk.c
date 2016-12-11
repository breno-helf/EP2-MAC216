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

int read_l (FILE *input, Buffer *buffer)
{
    buffer_reset(buffer);
    char c = getc(input);
    while (c != '\n' && c != EOF) {
        buffer_push_back(buffer, c);
        c = getc(input);
    }
    buffer_push_back(buffer, '\0');
    if (buffer->i == 1) {
        if (c == EOF)
            return -1;
        return 0;
    }
    return (buffer->i - 1);
}

int main (int argc, char *argv[]) {
	int i, line,  num_lines, j, k, l;
	FILE *in, *out, *aux;
	char *infile, *execfile, *buffer_aux, c;
	Buffer *buffer;
	buffer = buffer_create();
	buffer_aux = malloc (sizeof (char) * 10);
	SymbolTable extern_table;
	InsertionResult res;
	extern_table = stable_create();
	execfile = argv[1];
	out = fopen(execfile, "w");
	in = fopen("temp.txt", "w+");
	fprintf(in, "* 72 main\n");
	num_lines = 1;
	for (i = 2; i < argc; i++) {
	/*lê arquivos de entrada, lê externs e coloca
	na extern_table, além de calcular a linha a que
	cada label se refere, no arquivo montado,
	acusa erro se o mesmo label for definido mais de
	uma vez*/
		infile = argv[i];
		aux = fopen(infile, "r+");
		while ((c = getc(aux)) != EOF)
			fprintf(in, "%c", c);
		rewind(in);
		for (j = 0; j < num_lines + (3 * (i - 2)); j++)
			read_l(in, buffer);
		read_l(in, buffer);
		l = atoi(buffer->data);
		read_l(in, buffer);
		while (buffer->data[0] == 'E') {
			k = 0;
			for (j = 2; buffer->data[j] != ' '; j++) {
				buffer_aux[k] = buffer->data[j];
				k++;
			}
			buffer_aux[k] = '\0';
			res.data = stable_find (extern_table, buffer_aux);
			if (res.data != NULL)
				die("%s label already defined\n", buffer_aux);
			res = stable_insert(extern_table, buffer_aux);
			res.data->i = atoi(&(buffer->data[j + 1])) + num_lines;
			read_l (in, buffer);
		}
		num_lines += l;
		while (read_l (in, buffer) != -1);
	}
	/*verifica se a main foi definida*/
	if (stable_find(extern_table, "main") == NULL)
		die("main not defined\n");
	rewind(in);
	i = line = 0;
	read_l (in, buffer);
	res.data = stable_find (extern_table, &(buffer->data[5]));
	k = (res.data->i) - line;
	fprintf(out, "48%.6X\n", k);
	line++;
    /*imprime os comandos na saída, calculando os jumps
    dos externs, dá erro se o jump por para algum extern
    não definido*/
	while (read_l (in, buffer) != -1) {
		i = atoi(buffer->data);
		while (buffer->data[0] != 'B')
			read_l (in, buffer);
		for (j = 0; j < i; j++) {
			read_l (in, buffer);
			if (buffer->data[0] == '*') {
				res.data = stable_find (extern_table, &(buffer->data[5]));
				if (res.data == NULL)
					die("%s not defined\n", &(buffer->data[5]));
				k = (res.data->i) - line;
				if (k < 0)
					fprintf(out, "49%.6x\n", -k);
				else
					fprintf(out, "48%.6x\n", k);
			}
			else
			    fprintf(out, "%s\n", buffer->data);
			line ++;
		}
	}
	remove("temp.txt");
	return 0;
}