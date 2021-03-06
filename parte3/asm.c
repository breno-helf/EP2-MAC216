/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: 8991769
    Raphael dos Reis Gusmao     NUSP: 9778561
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "stable.h"
#include "asm.h"
#include "error.h"
#include "mactypes.h"
#include "asmtypes.h"
#include "optable.h"
#include "opcodes.h"
#include "parser.h"
#include "buffer.h"

#define MAX_JMP 16777215

FILE *globalOut;

int isCode (Instruction *instr)
{
	return (instr->op->opcode != EXTERN && instr->op->opcode != IS);
}

int isPseudoCode (Instruction *instr)
{
	int opcode = instr->op->opcode;
	return (opcode == CALL || opcode == PUSH || opcode == RET);
}

Instruction *pseudoToCode (Instruction *instr)
{
	Instruction *ret, *aux;
    Operator *op;
    Operand *opds[3];
	int opcode = instr->op->opcode;

    /* CALL */
	if (opcode == CALL) {
		op = optable_find("GETA");
		opds[0] = operand_create_register(250);
		opds[1] = operand_create_number(4);
		opds[2] = NULL;
		ret = instr_create(instr->label, op, opds);
		aux = ret;

		op = optable_find("STOU");
		opds[0] = operand_create_register(250);
		opds[1] = operand_create_register(253);
		opds[2] = operand_create_number(0);
		aux->next = instr_create(NULL, op, opds);
		aux = aux->next;

		op = optable_find("ADDU");
		opds[0] = operand_create_register(253);
		opds[1] = operand_create_register(253);
		opds[2] = operand_create_number(8);
		aux->next = instr_create(NULL, op, opds);
		aux = aux->next;

		op = optable_find("JMP");
		opds[0] = instr->opds[0];
		opds[1] = NULL;
		opds[2] = NULL;
		aux->next = instr_create(NULL, op, opds);
		aux = aux->next;
		aux->next = NULL;
	}
    /* PUSH */
	else if (opcode == PUSH) {
		op = optable_find("STOU");
		opds[0] = instr->opds[0];
		opds[1] = operand_create_register(253);
		opds[2] = operand_create_number(0);
		ret = instr_create(instr->label, op, opds);

		op = optable_find("ADDU");
		opds[0] = operand_create_register(253);
		opds[1] = operand_create_register(253);
		opds[2] = operand_create_number(8);
		aux = ret;
		aux->next = instr_create(NULL, op, opds);
		aux = aux->next;
		aux->next = NULL;
	}
    /* RET */
	else if (opcode == RET) {
		op = optable_find("SUBU");
		opds[0] = operand_create_register(253);
		opds[1] = operand_create_register(253);
		opds[2] = operand_create_number(8*(instr->opds[0]->value.num + 1));
		ret = instr_create(instr->label, op, opds);
		aux = ret;

		op = optable_find("LDOU");
		opds[0] = operand_create_register(250);
		opds[1] = operand_create_register(253);
		opds[2] = operand_create_number(8*(instr->opds[0]->value.num));
		aux->next = instr_create(NULL, op, opds);
		aux = aux->next;

		op = optable_find("GO");
		opds[0] = operand_create_register(250);
		opds[1] = operand_create_number(0);
		opds[2] = NULL;
		aux->next = instr_create(NULL, op, opds);
		aux = aux->next;
		aux->next = NULL;
	}
    /* Erro */
	else die("Voce usou a funcao de converter codigo em pseudocodigo em algo que nao eh pseudocodigo");

    return ret;
}

void set_globalOut (FILE *output)
{
	globalOut = output;
}

int outPrint (const char *key, EntryData *data)
{
	if (key == NULL || data == NULL) return 0;
	if (data->i == -1)
		die("Label %s never defined\n", key);
	fprintf(globalOut, "E %s %d\n", key, data->i);
	return 1;
}

int assemble (const char *filename, FILE *input, FILE *output) {
	/*
        A alias type guarda um operand do tipo REG
        A label guarda a linha em que a string está
        A extern guarda o numero do EXTERN
	*/
	SymbolTable alias_table, label_table, extern_table;
    Instruction *start, *cur;
    int line, aux, f = 0, pos = 0, i;
	Buffer *buffer;

    /* Inicializacoes */
	buffer = buffer_create();
	alias_table = stable_create();
	label_table = stable_create();
	extern_table = stable_create();

    /* Default */
    unsigned char regs[] = {255,  254,  253,   252,   251, 250};
    char *regLabels[] =    {"rA", "rR", "rSP", "rX", "rY", "rZ"};
    for (int i = 0; i < 6; i++) {
        Operand *opd = operand_create_register(regs[i]);
        InsertionResult res = stable_insert(alias_table, regLabels[i]);
        res.data->opd = opd;
    }

	line = 0;
	aux = read_line(input, buffer);
	while (aux != -1) {
		if (aux > 0) {
			const char *errptr;
			Instruction *instr = malloc(sizeof(Instruction));
			/* Instrucao */
			if (parse(buffer->data, alias_table, &instr, &errptr)) {
				line++;
				if (instr != NULL) {
					instr->next = NULL;
					/* IS */
					if (instr->op->opcode == IS) {
						EntryData *label_ptr, *alias_ptr;

						label_ptr = stable_find(label_table, instr->label);
						alias_ptr = stable_find(alias_table, instr->label);

						if (label_ptr == NULL && alias_ptr == NULL && checkRotulo(instr->label, errptr)) {
							Operand *opd = operand_create_register(instr->opds[0]->value.reg);
							InsertionResult res = stable_insert(alias_table, instr->label);
							res.data->opd = opd;
						} else {/* Erro */
							fprintf(stderr, "line %d: %s\n", line, buffer->data);
							if (label_ptr != NULL && alias_ptr != NULL) {
								fprintf(stderr, "        ");
								for (char *p = buffer->data; p != errptr; p++)
									fprintf(stderr, " ");
				                fprintf(stderr, "^\n");
							}
			    			exit(1);
						}
					}
					/* Label */
					else if (instr->label != NULL) {
						EntryData *label_ptr, *alias_ptr, *extern_ptr;
						InsertionResult res;

						label_ptr = stable_find(label_table, instr->label);
						alias_ptr = stable_find(alias_table, instr->label);
						extern_ptr = stable_find(extern_table, instr->label);

						if (instr->op->opcode == EXTERN)
							die("%s\nlabel %s should not be defined for EXTERN\n", buffer->data, instr->label);

						if (label_ptr != NULL || alias_ptr != NULL)
							die("%s\nlabel %s already defined", buffer->data, instr->label);

						if (extern_ptr)
							extern_ptr->i = pos;

						res = stable_insert(label_table, instr->label);
						res.data->i = pos;
					}
					/* EXTERN */
					else if (instr->op->opcode == EXTERN) {
						EntryData *label_ptr, *alias_ptr, *extern_ptr;
						InsertionResult res;
						char *label;
						label = instr->opds[0]->value.str;

						label_ptr = stable_find(label_table, label);
						alias_ptr = stable_find(alias_table, label);
						extern_ptr = stable_find(extern_table, label);

						if (label_ptr != NULL || alias_ptr != NULL || extern_ptr != NULL) {
							fprintf(stderr, "%s\nlabel %s already defined", buffer->data, instr->label);
							exit(-1);
						}

						res = stable_insert(extern_table, label);
						res.data->i = -1; /* numero do EXTERN inserido */
					}

					/* Fazendo a lista ligada */
					if (f == 0 && isCode(instr)) {
						if (isPseudoCode(instr))
							instr = pseudoToCode(instr);

						instr->pos = pos++;
						instr->lineno = line;
						start = cur = instr;

						for (cur = cur; cur->next != NULL; cur = cur->next) {
							cur->next->pos = pos++;
							cur->next->lineno = line;
						}

						f = 1;
					} else if (isCode(instr)) {
						if (isPseudoCode(instr))
							instr = pseudoToCode(instr);

						instr->pos = pos++;
						instr->lineno = line;
						cur->next = instr;
						cur = instr;

						for (cur = cur; cur->next != NULL; cur = cur->next) {
							cur->next->pos = pos++;
							cur->next->lineno = line;
						}
					}
				}
			}
		}
		aux = read_line(input, buffer);
	}
	buffer_destroy(buffer);

	/* Percorre a lista gerando o codigo objeto de cada instrucao */
	fprintf(output, "%d\n", pos);
	set_globalOut(output);
	stable_visit(extern_table, outPrint);
	fprintf(output, "B\n");
	for (cur = start; cur != NULL; cur = cur->next) {
		/*
		  Imprime os opcodes e os operandos em Hexadecimal
		  ( %x para o printf ).
		*/
		int count = 0;
		f = 0;

		if (cur->op->opcode == JMP) {
			EntryData *label_ptr;
			label_ptr = stable_find(label_table, cur->opds[0]->value.label);
			if (label_ptr == NULL) {
				fprintf(output, "* 72 %s\n", cur->opds[0]->value.label);
				continue;
			}
		}

		if (cur->op->opcode >= 0x48 && cur->op->opcode <= 0x55) {
			int cur_line, jmp_line;
			EntryData *label_ptr;
			cur_line = cur->pos;
			if (cur->op->opcode == 0x48) {
				if (cur->opds[0]->type != LABEL)
					goto out;
				i = 0;
			} else {
				if (cur->opds[1]->type != LABEL)
					goto out;
				i = 1;
			}
			label_ptr = stable_find(label_table, cur->opds[i]->value.label);

			if (label_ptr == NULL)
				die("%s label not defined", cur->opds[i]->value.label);
			jmp_line = label_ptr->i;

			if (cur_line > jmp_line)
				f = 1;

		} else {
			if (cur->opds[2]) {
				if ((cur->op->opcode%2 == 0) && cur->opds[2]->type == NUMBER_TYPE)
					f = 1;
			}
		}

	out:

		count += fprintf(output, "%.2x", cur->op->opcode + f);

		for (int i = 0; i < 3; i++) {
			if (cur->opds[i]) {
				if ((cur->opds[i]->type & LABEL) == LABEL) {
					int cur_line, jmp_line;
					EntryData *label_ptr;
					cur_line = cur->pos;
					label_ptr = stable_find(label_table, cur->opds[i]->value.label);


					if (label_ptr == NULL)
						die("%s label not defined", cur->opds[i]->value.label);
					else
						jmp_line = label_ptr->i;
					if (jmp_line > cur_line) {
						if ((jmp_line - cur_line) > MAX_JMP)
							die("Max JMP exceeded");
						if (cur->opds[1] == NULL)
							count += fprintf(output, "%.6x", jmp_line - cur_line);
						else if (cur->opds[2] == NULL)
							count += fprintf(output, "%.4x", jmp_line - cur_line);
						else
							count += fprintf(output, "%.2x", jmp_line - cur_line);
					} else {
						if ((cur_line - jmp_line) > MAX_JMP)
							die("Max JMP exceeded");

						if (cur->opds[1] == NULL)
							count += fprintf(output, "%.6x",  cur_line - jmp_line);

						else if (cur->opds[2] == NULL)
							count += fprintf(output, "%.4x",  cur_line - jmp_line);
						else
							count += fprintf(output, "%.2x",  cur_line - jmp_line);
					}
				}
				else if ((cur->opds[i]->type & STRING) == STRING)
					count += fprintf(output, "");
				else if ((cur->opds[i]->type & REGISTER) == REGISTER)
					count += fprintf(output, "%.2x", cur->opds[i]->value.reg);
				else {
					if (cur->opds[1] == NULL)
						count += fprintf(output, "%.6x", cur->opds[i]->value.num);
					else if (cur->opds[2] == NULL)
						count += fprintf(output, "%.4x", cur->opds[i]->value.num);
					else
						count += fprintf(output, "%.2x", cur->opds[i]->value.num);
				}
			}
		}
		fprintf(output, "\n");
	}

	stable_destroy(alias_table);
	stable_destroy(label_table);
	stable_destroy(extern_table);

	return 1;
}
