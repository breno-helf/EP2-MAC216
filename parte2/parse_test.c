#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "error.h"
#include "asmtypes.h"
#include "stable.h"
#include "buffer.h"
#include "error.h"




int main(int argc, char *argv[]) {
	SymbolTable table;
	FILE *in;
	Buffer *buffer = buffer_create();
	set_prog_name("Parse test");
	if(argc != 2)
		die("Numero de argumentos errado.");
	
	in = fopen(argv[1], "r");

	table = stable_create();

	while(read_line(in, buffer)) {
		Instruction *instr;
		char *errptr;
		instr = malloc(sizeof(Instruction));
		errptr = malloc(sizeof(char));
		if(parse(buffer->data, table, &instr, &errptr)) {
			printf("line = %s\n", buffer->data);
			printf("label =");
			if(label == NULL)
				printf(" n/a\n");
			else
				printf(" '%s' \n", instr->label);
			printf("operator = %s\n", instr->op->name);
			if(instr->op->opcode == IS) {
				InsertionResult res;
				stable_insert(table, buffer->data);
				res->data->

			}
		}
		else break;
	}
	return 0;
}







