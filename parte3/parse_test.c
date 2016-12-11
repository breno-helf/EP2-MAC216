/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: 8991769
    Raphael dos Reis Gusmao     NUSP: 9778561
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "asmtypes.h"
#include "error.h"
#include "stable.h"
#include "parser.h"
#include "buffer.h"

int check_char (char c)
{
    return ((c > 64 && c < 91) || (c == 95) || (c > 96 && c < 123) || (c > 47 && c < 58));
}

int chk_rotulo(char *stg, const char *errptr)
{
    int i = 1, j;
    if (!(isalpha(stg[0]) || stg[0] == 95)) {
        errptr = &stg[0];
		set_error_msg("primeiro char nao eh letra ou underscore");
		return 0;
    }
    while (stg[i] != '\0') {
        j = check_char(stg[i]);
        if (!j) {
			errptr = &stg[i];
			set_error_msg("Encontrado char que nao eh numero, letra ou underscore");
			return 0;
        }
        i++;
    }
    return 1;
}

int main (int argc, char *argv[])
{
	int line, aux;
	FILE *in;
	Buffer *buffer;
	SymbolTable alias_table;

	set_prog_name("parse_test");

	if (argc != 2)
		die("Numero de argumentos errado.");

	in = fopen(argv[1], "r");

	buffer = buffer_create();
	alias_table = stable_create();

    /* Default */
    unsigned char regs[] = {255,  254,  253,   252,   251, 250};
    char *regLabels[] =    {"rA", "rR", "rSP", "rX", "rY", "rZ"};
    for (int i = 0; i < 6; i++) {
        Operand *opd = operand_create_register(regs[i]);
        InsertionResult res = stable_insert(alias_table, regLabels[i]);
        res.data->opd = opd;
    }

	line = 0;
	aux = read_line(in, buffer);
	while (aux != -1) {
        if (aux > 0) {
    		const char *errptr;
    		Instruction *instr = malloc(sizeof(Instruction));
    		line++;
    		if (parse(buffer->data, alias_table, &instr, &errptr)) {
    			/* Line */
    			printf("line     = %s\n", buffer->data);
    			if (instr != NULL) {
    				/* IS */
    				if (instr->op->opcode == IS) {
    					if(checkRotulo(instr->label, errptr)) {
    						Operand *opd = operand_create_register(instr->opds[0]->value.reg);
    						InsertionResult res = stable_insert(alias_table, instr->label);
    						res.data->opd = opd;
    					} else {
    						printf("line     %d: %s\n", line, buffer->data);
    						printf("^\n");
    						print_error_msg(NULL);
    						exit(1);
    					}
    				}
    				/* Label */
    				if (instr->label != NULL)
    					printf("label    = \"%s\"\n", instr->label);
    				else
    					printf("label    = n/a\n");
    				/* Operator */
    				printf("operator = %s\n", instr->op->name);
    				/* Operands */
    				printf("operands = ");
    				for (int i = 0; i < 3; i++) {
    					if (instr->opds[i]) {
    						if (i != 0) printf(", ");
                            if ((instr->opds[i]->type & LABEL) == LABEL)
                                printf("Label(\"%s\")", instr->opds[i]->value.label);
                            else if ((instr->opds[i]->type & STRING) == STRING)
                                printf("String(\"%s\")", instr->opds[i]->value.str);
                            else if ((instr->opds[i]->type & REGISTER) == REGISTER)
                                printf("Register(%u)", instr->opds[i]->value.reg);
    						else
                                printf("Number(%lld)", instr->opds[i]->value.num);
    					}
    				}
    				printf("\n\n");
    			} else printf("label = n/a\noperator = n/a\noperands = n/a\n\n");
    		} else {
    			printf("line %d: %s\n", line, buffer->data);
    			printf("        ", errptr, *errptr);
                errptr = &(buffer->data[8]);
                for (char *p = &(buffer->data); p != errptr; p++) printf(" ");
                printf("^\n");
                printf("p:%p, *p:%d\n\n\n", errptr, *errptr);
    			print_error_msg(NULL);
    			exit(1);
    		}
        }
        aux = read_line(in, buffer);
	}

	buffer_destroy(buffer);
	stable_destroy(alias_table);

	return 0;
}
