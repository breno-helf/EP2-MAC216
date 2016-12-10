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
#include "parser.h"
#include "opcodes.h"

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

int isCode(Instruction *instr) {
	if(instr->op->opcode < 0)
		return 0;
	else
		return 1;
}

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
			/* Não sei se é assim que conta linhas, verificar
			[COMENTARIO DO LUCAS]
			Não é, porque tem instrução que soma mais de uma linha,
			tem instrução que não soma nada
			[COMENTARIO DO BRENO]
			Então faz ae, por que eu não manjo qual que conta qual
			que não
			*/
			line++;
			if (parse(buffer->data, alias_table, &instr, &errptr)) {
				/* Line */
				/* printf("line = %s\n", buffer->data); */
				if (instr != NULL) {
					/* IS */
					if (instr->op->opcode == IS) {
						/*Diferenciar mensagens de erro para cada erro */
						EntryData *label_ptr, *alias_ptr;
						label_ptr = stable_find(label_table, instr->label);
						alias_ptr = stable_find(alias_table, instr->label);
						if(label_ptr == NULL && alias_ptr == NULL && chk_rotulo(instr->label, errptr)) {
							Operand *opd = operand_create_register(instr->opds[0]->value.reg);
							InsertionResult res = stable_insert(alias_table, instr->label);
							res.data->opd = opd;
						} else {
							printf("line %d: %s\n", line, buffer->data);
							printf("^\n");
							print_error_msg(NULL);
							exit(-1);
						}
					}
					/* Label
					[COMENTÁRIO DO LUCAS] DONE
					acho que tem que ser else if, porque se
					não os casos de IS entram aqui também,
					talvez tenham que entrar e eu não sei*/
					else if (instr->label != NULL) {
						InsertionResult res;
						EntryData *label_ptr, *alias_ptr;
						label_ptr = stable_find(label_table, instr->label);
						alias_ptr = stable_find(alias_table, instr->label);
						if(instr->op->opcode == EXTERN) {
							printf("%s\nlabel %s should not be defined for EXTERN\n", buffer->data, instr->label);
						}
						
						if(label_ptr != NULL && alias_ptr != NULL) {							
							printf("%s\nlabel %s already defined", buffer->data, instr->label);
							exit(-1);
						}
						res = stable_insert(label_table, instr->label);
						res.data->str = instr->label;
					}
					
				}
			} 
			
		}
		/* Fazendo a lista ligada
		[COMENTÁRIO DO LUCAS] DONE
		Nem todas as instruções entram na lista ligada
		algumas são pseudocódigo, tipo IS */		
		if(f == 0) {
			start = cur = instr;
			f = 1;
		}
		else if(isCode(instr)) {
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
