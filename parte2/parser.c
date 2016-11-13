#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "asmtypes.h"
#include "error.h"
#include "stable.h"
#include "optable.h"

/*
    Checa se o caracter eh letra, numero ou _
*/
int check_carac (char c)
{
    return ((c > 64 && c < 91) || (c == 95) || (c > 96 && c < 123) || (c > 47 && c < 58));
}

int check_rotulo (char *stg, char *errptr)
{
    int i = 1, j;
    if (!(check_carac(stg[0]))) {
        errptr = &stg[0];
		printf("Econtrado char %c que nao eh numero, letra ou underscore\n", stg[0]);
		return 0;
    }
    while (stg[i] != '\0') {
        j = check_carac(stg[i]);
        if (!j) {
			errptr = &stg[i];
			printf("Encontrado char %c que nao eh numero, letra ou underscore", stg[i]);
			return 0;
        }
        i++;
    }
    return 1;
}

int le_str (char *s, char *errptr, SymbolTable table, Instruction *instr)
{
    char *rotulo, *operador, *operand, **opd_read;
    Operator *op;
	Operand **opd;
    int i, j, a;

	j = 0;
	opd = malloc(3 * sizeof(Operand *));
	for (i = 0; i < 3; i++)
		opd[i] = malloc(sizeof(Operand));
    opd_read = malloc(3 * sizeof(char *));
	for (i = 0; i < 3; i++)
		opd_read[i] = malloc(sizeof(char) * 16);

    rotulo = malloc(sizeof(char) * 16);
    for (i = 0; s[i] == ' '; i++);
    while (s[i] != ' ' && s[i] != '\0' && j < 16) {
        rotulo[j] = s[i];
        j++; i++;
    }
    if (j == 16) {
        printf("Rotulo %s invalido\n", rotulo);
        errptr = rotulo;
        return 0;
    }

    rotulo[j] = '\0';
    if (check_rotulo(rotulo, errptr) == 0) {
		printf("Rotulo %s invalido\n", rotulo);
        return 0;
    }

	if ((op = optable_find(rotulo)) != 0) {
        instr->op = op; /* A funcao optable_find ja faz isso */
		rotulo = NULL;
        goto operando;
	}

	if (stable_find(table, rotulo) != NULL) {
        printf("Rotulo %s ja existe\n", rotulo);
        errptr = rotulo;
        return 0;
    }

    instr->label = rotulo;
    operador = malloc(sizeof(char) * 16);
    for (; s[i] == ' '; i++);
    j = 0;
    while (s[i] != ' ' && s[i] != '\0') {
        operador[j] = s[i];
        j++; i++;
    }

	if ((op = optable_find(operador)) != 0) {
        instr->op = op;
	}

	else {
		printf("%s Nao eh operador\n", operador);
		errptr = operador;
		return 0;
	}

 operando:
    for (a = 0; a < 3; a++)
        instr->opds[a]->type = instr->op->opd_types[a];
	for (; s[i] == ' '; i++);
    if (instr->opds[0]->type != OP_NONE) {
	    j = 0;
        while (s[i] != ' ' && s[i] != '\0' && s[i] != ',') {
            opd_read[0][j] = s[i];
            j++; i++;
        }
        if (instr->opds[0]->type == LABEL) {
        	instr->opds[0] = operand_create_label(opd_read[0]);

        } else if (instr->opds[0]->type == REGISTER) {
            instr->opds[0] = operand_create_register(atoi(opd_read[0]));

        } else if (instr->opds[0]->type == STRING) {
        	instr->opds[0] = operand_create_string(opd_read[0]);

        } else if (instr->opds[0]->type == NUMBER_TYPE) {
        	instr->opds[0] = operand_create_number(atoi(opd_read[0]));

        }
    }
    if (instr->opds[1]->type != OP_NONE) {
	    for (; s[i] == ' ' || s[i] == ','; i++);
	    j = 0;
        while (s[i] != ' ' && s[i] != '\0' && s[i] != ',') {
            opd_read[1][j] = s[i];
            j++; i++;
        }
        if (instr->opds[1]->type == LABEL) {
        	instr->opds[1] = operand_create_label(opd_read[1]);

        } else if (instr->opds[1]->type == REGISTER) {
            instr->opds[1] = operand_create_register(atoi(opd_read[1]));

        } else if (instr->opds[1]->type == STRING) {
        	instr->opds[1] = operand_create_string(opd_read[1]);

        } else if (instr->opds[1]->type == NUMBER_TYPE) {
        	instr->opds[1] = operand_create_number(atoi(opd_read[1]));

        }
    }
    if (instr->opds[2]->type != OP_NONE) {
	    for (; s[i] == ' ' || s[i] == ','; i++);
	    j = 0;
        while (s[i] != ' ' && s[i] != '\0' && s[i] != ',') {
            opd_read[2][j] = s[i];
            j++; i++;
        }
        if (instr->opds[2]->type == LABEL) {
        	instr->opds[2] = operand_create_label(opd_read[2]);

        } else if (instr->opds[2]->type == REGISTER) {
            instr->opds[2] = operand_create_register(atoi(opd_read[2]));

        } else if (instr->opds[2]->type == STRING) {
        	instr->opds[2] = operand_create_string(opd_read[2]);

        } else if (instr->opds[2]->type == NUMBER_TYPE) {
        	instr->opds[2] = operand_create_number(atoi(opd_read[2]));

        }
    }
    for (; s[i] != '\0' || s[i] != '*'; i++)
        if (s[i] != ' ' || s[i] != '\n') {
            printf("Erro de sintaxe: %c\n", s[i]);
            errptr = &(s[i]);
            return 0;
        }
    return 1;
}

int parse(const char *s, SymbolTable alias_table, Instruction **instr, const char **errptr) {
	/*
	  Aqui fica o monstro BIRRRL
	*/
    return 0;
}
