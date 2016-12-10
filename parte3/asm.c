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
	if(instr->op->opcode != EXTERN && instr->op->opcode != IS)
		return 0;
	else
		return 1;
}

int isPseudoCode(Instruction *instr) {
	int var;
	var = instr->op->opcode;
	return (var == CALL || var == PUSH || var == RET);
}

Instruction* PseudoToCode(Instruction *instr) {
	Instruction *ret, *aux;
	int var;
	var = instr->op->opcode;
	if(var == CALL) {
		Operator *op;
		Operand *opds[3];
		
		op = optable_find("GETA");
		opds[0] = operand_create_register(250);
		opds[1] = operand_create_number(4);
		opds[2] = NULL;
		ret = instr_create(instr->label, op, opds);
		aux = ret;
		
		op = optable_find("STO");
		opds[0] = instr->opds[0];
		opds[1] = operand_create_register(253);
		opds[2] = operand_create_number(0);
		aux->next = instr_create(NULL, op, opds);
		aux = aux->next;

		op = optable_find("ADD");
		opds[0] = operand_create_register(253);
		opds[1] = operand_create_register(253);
		opds[2] = operand_create_number(8);				
		aux->next = instr_create(NULL, op, opds);
		aux = aux->next;

		op = optable_find("JMP");
		opds[0] = instr->op->opds[0];
		opds[1] = NULL;
		opds[2] = NULL;						
		aux->next = instr_create(NULL, op, opds);
		aux = aux->next;
		aux->next = NULL;
		return ret;
	}
	else if(var == PUSH) {
		Operator *op;
		Operand *opds[3];

		op = optable_find("STO");
		opds[0] = instr->opds[0];
		opds[1] = operand_create_register(253);
		opds[2] = operand_create_number(0);
		ret = instr_create(instr->label, op, opds);
		
		op = optable_find("ADD");
		opds[0] = operand_create_register(253);
		opds[1] = operand_create_register(253);
		opds[2] = operand_create_number(8);
		aux = ret;
		aux->next = instr_create(NULL, op, opds);
		aux = aux->next;
		aux->next = NULL;
		return ret;
	}
	else if(var == RET) {
		Operator *op;
		Operand *opds[3];
		
		op = optable_find("SUB");
		opds[0] = operand_create_register(253);
		opds[1] = operand_create_register(253);
		opds[2] = operand_create_number(8*(instr->opds[0]->OperandValue.num + 1))
		ret = instr_create(instr->label, op, opds);
		aux = ret;
		
		op = optable_find("LDO");
		opds[0] = operand_create_register(250);
		opds[1] = operand_create_register(253);
		opds[2] = operand_create_number(8*(instr->opds[0]->OperandValue.num));
		aux->next = instr_create(NULL, op, opds);
		aux = aux->next;

		op = optable_find("GO");
		opds[0] = operand_create_register(250);
		opds[1] = operand_create_number(0);
		opds[2] = NULL;				
		aux->next = instr_create(NULL, op, opds);
		aux = aux->next;
		aux->next = NULL;
		return ret;
	}
	else {
		die("Você usou a função de converter código em pseudocódigo em algo que não é pseudocódigo");
		return instr;
	}
	
	
}

/*
  Implementar a função
*/

int assemble(const char *filename, FILE *input, FILE *output) {
	/* A alias type guarda um operand do tipo REG
	   A label guarda a linha em que a string está
	   A extern guarda apenas se aquele label EXTERN existe
	*/
	SymbolTable alias_table, label_table, extern_table;
    unsigned char regs[] = {255,  254,  253,   252,   251, 250};
    char *regLabels[] =    {"rA", "rR", "rSP", "rX", "rY", "rZ"};
	alias_table = stable_create();
	label_table = stable_create();
	extern_table = stable_create();
    for (int i = 0; i < 6; i++) {
        Operand *opd = operand_create_register(regs[i]);
        InsertionResult res = stable_insert(alias_table, regLabels[i]);
        res.data->opd = opd;
	}
	/*
	  Usar o parser para fazer a lista ligada de instruções
	  Lembre-se de sempre atualizar as tabelas de simbolo
	  acima quando necessário.
	*/
    
    Instruction *start, *cur;
    int line, aux, f = 0, pos = 0;
	
	line = 0;
	aux = read_line(input, buffer);
	while (aux != -1) {
		if (aux > 0) {
			const char *errptr;
			Instruction *instr;		 
			if (parse(buffer->data, alias_table, &instr, &errptr)) {				
				line++;
				if (instr != NULL) {
					instr->next = NULL;
					/* IS */
					if (instr->op->opcode == IS) {
						/*Diferenciar mensagens de erro para cada erro */
						EntryData *label_ptr, *alias_ptr, *extern_ptr;
						label_ptr = stable_find(label_table, instr->label);
						alias_ptr = stable_find(alias_table, instr->label);
						extern_ptr = stable_find(extern_table, instr->label);
						if(label_ptr == NULL && alias_ptr == NULL && chk_rotulo(instr->label, errptr)) {
							Operand *opd = operand_create_register(instr->opds[0]->value.reg);
							InsertionResult res = stable_insert(alias_table, instr->label);
							res.data->opd = opd;
						} else {
							fprintf(stderr, "line %d: %s\n", line, buffer->data);
							fprintf(stderr, "^\n");
							print_error_msg(NULL);
							exit(-1);
						}
					}
					/* 
					   Label
					*/
					else if (instr->label != NULL) {
						InsertionResult res;
						EntryData *label_ptr, *alias_ptr, *extern_ptr;
						label_ptr = stable_find(label_table, instr->label);
						alias_ptr = stable_find(alias_table, instr->label);
						extern_ptr = stable_find(extern_table, instr->label);
						if(instr->op->opcode == EXTERN) {
							fprintf(stderr, "%s\nlabel %s should not be defined for EXTERN\n", buffer->data, instr->label);
							exit(-1);
						}
						
						if(label_ptr != NULL || alias_ptr != NULL || extern_ptr != NULL) {							
							fprintf(stderr, "%s\nlabel %s already defined", buffer->data, instr->label);
							exit(-1);
						}
						res = stable_insert(label_table, instr->label);
						res.data->i = line;
					}
					else {
						if(instr->op->opcode == EXTERN) {
							InsertionResult res;
							EntryData *label_ptr, *alias_ptr, *extern_ptr;
							char *label;
							label = instr->opds[0]->value.str;
							label_ptr = stable_find(label_table, label);
							alias_ptr = stable_find(alias_table, label);
							extern_ptr = stable_find(extern_table, label);
							if(label_ptr != NULL || alias_ptr != NULL || extern_ptr != NULL) {							
								fprintf(stderr, "%s\nlabel %s already defined", buffer->data, instr->label);
								exit(-1);
							}
							res = stable_insert(extern_table, label);
							res->data->i = 1; /* Boolean para dizer que esse label extern existe */
						}
					}
					/* 
					   Fazendo a lista ligada
					*/
					if(f == 0 && isCode(instr)) {
						if(isPseudoCode(instr))
							instr = PseudoToCode(instr);

						instr->pos = pos++;
						instr->lineno = line;
						start = cur = instr;
						for(cur = cur; cur->next != NULL; cur = cur->next) {}
						f = 1;
					}
					else if(isCode(instr)) {
						if(isPseudoCode(instr))
							instr = PseudoToCode(instr);

						instr->pos = pos++;
						instr->lineno = line;						
						cur->next = instr;
						cur = instr;
						for(cur = cur; cur->next != NULL; cur = cur->next) {}

					}
					
				}
				aux = read_line(input, buffer);
			}
		}
	}
	buffer_destroy(buffer);

	/*
	  Após isso percorra a lista gerando o código objeto de cada
	  instrução, definido por nós mesmos :P
	*/
	for(cur = start; cur->next != NULL; cur = cur->next) {
		/*
		  Imprime os opcodes e os operandos em Hexadecimal
		  ( %x para o printf ).
		*/
		
		printf("%x", cur->op->opcode, 
	}
}
