#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
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

int check_rotulo (char *stg, const char *errptr)
{
    int i = 1, j;
    if (!(isalpha(stg[0]) || stg[0] == 95)) {
        errptr = &stg[0];
		set_error_msg("primeiro char nao eh letra ou underscore");
		return 0;
    }
    while (stg[i] != '\0') {
        j = check_carac(stg[i]);
        if (!j) {
			errptr = &stg[i];
			set_error_msg("Encontrado char que nao eh numero, letra ou underscore");
			return 0;
        }
        i++;
    }
    return 1;
}

int parse(const char *s, SymbolTable alias_table, Instruction **instr, const char **errptr) {
    char *rotulo, *operador, *operand, **opd_read;
    const Operator *op;
	Operand *opd[3];
    int i, j, a;
	j = 0;
	(*instr) = malloc(sizeof(Instruction));
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
        set_error_msg("Rotulo invalido");
        *errptr = rotulo;
        return 0;
    }

    rotulo[j] = '\0';
    if (check_rotulo(rotulo, *errptr) == 0) {
		set_error_msg("Rotulo invalido");
        return 0;
    }

	if ((op = optable_find(rotulo)) != 0) {
        (*instr)->op = op; /* A funcao optable_find ja faz isso */
		rotulo = NULL;
        goto operando;
	}

	if (stable_find(alias_table, rotulo) != NULL) {
        set_error_msg("Rotulo ja existe");
        *errptr = rotulo;
        return 0;
    }

    (*instr)->label = rotulo;
    operador = malloc(sizeof(char) * 16);
    for (; s[i] == ' '; i++);
    j = 0;
    while (s[i] != ' ' && s[i] != '\0') {
        operador[j] = s[i];
        j++; i++;
    }

	if ((op = optable_find(operador)) != 0) {
        (*instr)->op = op;
	}

	else {
		set_error_msg("Nao eh operador");
		*errptr = operador;
		return 0;
	}

 operando:
    for (a = 0; a < 3; a++)
        (*instr)->opds[a]->type = (*instr)->op->opd_types[a];
	for (; s[i] == ' '; i++);
    if ((*instr)->opds[0]->type != OP_NONE) {
	    j = 0;
        while (s[i] != ' ' && s[i] != '\0' && s[i] != ',') {
            opd_read[0][j] = s[i];
            j++; i++;
        }
        if ((*instr)->opds[0]->type == LABEL) {
        	(*instr)->opds[0] = operand_create_label(opd_read[0]);

        } else if ((*instr)->opds[0]->type == REGISTER) {
            (*instr)->opds[0] = operand_create_register(atoi(opd_read[0]));

        } else if ((*instr)->opds[0]->type == STRING) {
        	(*instr)->opds[0] = operand_create_string(opd_read[0]);

        } else if ((*instr)->opds[0]->type == NUMBER_TYPE) {
        	(*instr)->opds[0] = operand_create_number(atoi(opd_read[0]));

        }
    }
    if ((*instr)->opds[1]->type != OP_NONE) {
	    for (; s[i] == ' ' || s[i] == ','; i++);
	    j = 0;
        while (s[i] != ' ' && s[i] != '\0' && s[i] != ',') {
            opd_read[1][j] = s[i];
            j++; i++;
        }
        if ((*instr)->opds[1]->type == LABEL) {
        	(*instr)->opds[1] = operand_create_label(opd_read[1]);

        } else if ((*instr)->opds[1]->type == REGISTER) {
            (*instr)->opds[1] = operand_create_register(atoi(opd_read[1]));

        } else if ((*instr)->opds[1]->type == STRING) {
        	(*instr)->opds[1] = operand_create_string(opd_read[1]);

        } else if ((*instr)->opds[1]->type == NUMBER_TYPE) {
        	(*instr)->opds[1] = operand_create_number(atoi(opd_read[1]));

        }
    }
    if ((*instr)->opds[2]->type != OP_NONE) {
	    for (; s[i] == ' ' || s[i] == ','; i++);
	    j = 0;
        while (s[i] != ' ' && s[i] != '\0' && s[i] != ',') {
            opd_read[2][j] = s[i];
            j++; i++;
        }
        if ((*instr)->opds[2]->type == LABEL) {
        	(*instr)->opds[2] = operand_create_label(opd_read[2]);

        } else if ((*instr)->opds[2]->type == REGISTER) {
            (*instr)->opds[2] = operand_create_register(atoi(opd_read[2]));

        } else if ((*instr)->opds[2]->type == STRING) {
        	(*instr)->opds[2] = operand_create_string(opd_read[2]);

        } else if ((*instr)->opds[2]->type == NUMBER_TYPE) {
        	(*instr)->opds[2] = operand_create_number(atoi(opd_read[2]));

        }
    }
    for (; s[i] != '\0' || s[i] != '*'; i++)
        if (s[i] != ' ' || s[i] != '\n') {
            set_error_msg("Erro de sintaxe");
            *errptr = &(s[i]);
            return 0;
        }
    return 1;
}
