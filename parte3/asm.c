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
    
    Instruction *start, *cur;
    int line, aux, f = 0;
	
	line = 0;
	aux = read_line(input, buffer);
	while (aux != -1) {
		if (aux > 0) {
			const char *errptr;
			Instruction *instr;
			/* Não sei se é assim que conta linhas, verificar */
			line++;
			if (parse(buffer->data, alias_table, &instr, &errptr)) {
				/* Line */
				/* printf("line = %s\n", buffer->data); */
				if (instr != NULL) {
					/* IS - Verificar se o alias já não foi definido*/
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
					/* Label - Verificar se Rotulo ja nao foi definido*/
					if (instr->label != NULL) {
						InsertionResult res;
						res = stable_insert(label_table, instr->label);
						res.data->str = instr->label;
					}
					
				}
			} 
			
		}
		/* Fazendo a lista ligada */		
		if(f == 0) {
			start = cur = instr;
			f = 1;
		}
		else {
			cur->next = insrt;
			cur = instr;
		}
		aux = read_line(input, buffer);
	}

	buffer_destroy(buffer);

		
	/*
	  Após isso percorra a lista gerando o código objeto de cada
	  instrução, definido por nós mesmos :P
	*/
}
