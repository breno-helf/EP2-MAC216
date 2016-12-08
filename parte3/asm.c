/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: 8991769
    Raphael dos Reis Gusmao     NUSP: 9778561
*/
#include<stdio.h>
#include<stlib.h>
#include "stable.h"
#include "asm.h"
#include "error.h"
#include "mactypes.h"
#include "asmtypes.h"
#include "optable.h"
#include "opcodes.h"

/*
  Implementar a função
*/

int assemble(const char *filename, FILE *input, FILE *output) {
	SymbolTable alias_table, label_table, extern_table;
	InsertionResult res;
	alias_table = stable_create();
	label_table = stable_create();
	extern_table = stable_create();
	res = stable_insert(alias_table, rA);
	res->data->s = 255
	res = stable_insert(alias_table, rR);
	res->data->s = 254;
	res = stable_insert(alias_table, rSP);
	res->data->s = 253;
	res = stable_insert(alias_table, rX);
	res->data->s = 252;
	res = stable_insert(alias_table, rY);
	res->data->s = 251;
	res = stable_insert(alias_table, rZ);
	res->data->s = 250;

	/*
	  Usar o parser para fazer a lista ligada de instruções
	  Lembre-se de sempre atualizar as tabelas de simbolo
	  acima quando necessário.
	*/
    
    Instruction *start, *curr;
    int line, aux;
	
	line = 0;
	aux = read_line(in, buffer);
	while (aux != -1) {
		if (aux > 0) {
			const char *errptr;
			Instruction *instr;
			line++;
			if (parse(buffer->data, alias_table, &instr, &errptr)) {
				/* Line */
				/* printf("line = %s\n", buffer->data); */
				if (instr != NULL) {
					/* IS */
					if (instr->op->opcode == IS) {
						if(chk_rotulo(instr->label, errptr)) {
							Operand *opd = operand_create_register(instr->opds[0]->value.reg);
							InsertionResult res = stable_insert(alias_table, instr->label);
							res.data->opd = opd;
						} else {
							printf("line %d: %s\n", line, buffer->data);
							printf("^\n");
							print_error_msg(NULL);
							exit(1);
						}
					}
					/* Label */
					/* if (instr->label != NULL) */

					/* 	else */
						
						/* Operator */

						/* Operands */
					/*						
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
					*/	
			} else {
				printf("line %d: %s\n", line, buffer->data);
				printf("^\n");
				print_error_msg(NULL);
				exit(1);
			}
				  
		}
		aux = read_line(in, buffer);
	}

	buffer_destroy(buffer);

	
    /*
    separa in em linhas, chama o parser, salva em curr
	if (curr == pseudoOP)
	    atualiza tabelas;
	else {
	    instr->next = curr
	    intr = curr
	    lns_objcode += n
	} 
	*/
	
	/*
	  Após isso percorra a lista gerando o código objeto de cada
	  instrução, definido por nós mesmos :P
	*/
}
