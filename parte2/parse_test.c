/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: 8991769
    Raphael dos Reis Gusmao     NUSP: 9778561
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "error.h"
#include "asmtypes.h"
#include "stable.h"
#include "buffer.h"
#include "error.h"

int main (int argc, char *argv[])
{
	FILE *in;
	Buffer *buffer;
	SymbolTable alias_table;

	set_prog_name("parse_test");

	if (argc != 2)
		die("Numero de argumentos errado.");

	in = fopen(argv[1], "r");

	buffer = buffer_create();
	alias_table = stable_create();

	while (read_line(in, buffer)) {
		const char *errptr;
		Instruction *instr;

		/*instr = malloc(sizeof(Instruction));
		errptr = malloc(sizeof(char));*/

		if (parse(buffer->data, alias_table, &instr, &errptr)) {
			/* Line */
			printf("line = %s\n", buffer->data);

			if (instr != NULL) {
				if (instr->op->opcode == IS) {
					
				}

				/* Label */
				if (instr->label != NULL)
					printf("label = \"%s\"\n", instr->label);
				else
					printf("label = n/a\n");

				/* Operator */
				printf("operator = %s\n", instr->op->name);

				/* Operands */
				printf("operands = ");
				for (int i = 0; i < 3; i++) {
					if (instr->opds[i]) {
		                if (i != 0) printf(", ");
		                switch (instr[l]->opds[i]->type) {
							case NUMBER_TYPE:
			                    printf("Number(%lld)", instr->opds[i]->value.num);
			                    break;
							case LABEL:
			                    printf("Label(\"%s\")", instr->opds[i]->value.label);
			                    break;
							case STRING:
			                    printf("String(%s)", instr->opds[i]->value.str);
			                    break;
			                case REGISTER:
			                    printf("Register(%u)", instr->opds[i]->value.reg);
			                    break;
		                }
		            }
				}
				printf("\n\n");
			} else {
				printf("label = n/a\n
						operator = n/a\n
						operands = n/a\n\n");
			}
		} else {
			/* Deu erro!!! */
			break;
		}
	}
	return 0;
}
