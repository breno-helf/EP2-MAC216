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
    for (i = 0; isspace(s[i]); i++);
    while (!(isspace(s[i])) && s[i] != '\0' && j < 16) {
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
	if(rotulo[0] != 0) {
		InsertionResult res = stable_insert(alias_table, rotulo);
		res.data->str = rotulo;
	}
	
    (*instr)->label = rotulo;
    operador = malloc(sizeof(char) * 16);
    for (; isspace(s[i]); i++);
    j = 0;
    while (!(isspace(s[i])) && s[i] != '\0') {
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
	for (; isspace(s[i]); i++);
    if ((*instr)->op->opd_types[0] != OP_NONE) {
	    j = 0;
        while (!(isspace(s[i])) && s[i] != '\0' && s[i] != ',') {
            opd_read[0][j] = s[i];
            j++; i++;
        }
        if (((*instr)->op->opd_types[0]&LABEL) == LABEL) {
        	(*instr)->opds[0] = operand_create_label(opd_read[0]);

        } else if (((*instr)->op->opd_types[0]&REGISTER) == REGISTER) {
            (*instr)->opds[0] = operand_create_register(atoi(opd_read[0]));

        } else if (((*instr)->op->opd_types[0]&STRING) == STRING) {
        	(*instr)->opds[0] = operand_create_string(opd_read[0]);

        } else {
        	(*instr)->opds[0] = operand_create_number(atoi(opd_read[0]));

        }
    }
    if ((*instr)->op->opd_types[1] != OP_NONE) {
	    for (; isspace(s[i]) || s[i] == ','; i++);
	    j = 0;
        while (!(isspace(s[i])) && s[i] != '\0' && s[i] != ',') {
            opd_read[1][j] = s[i];
            j++; i++;
        }
        if ((*instr)->op->opd_types[1] == LABEL) {
        	(*instr)->opds[1] = operand_create_label(opd_read[1]);

        } else if ((*instr)->op->opd_types[1] == REGISTER) {
            (*instr)->opds[1] = operand_create_register(atoi(opd_read[1]));

        } else if ((*instr)->op->opd_types[1] == STRING) {
        	(*instr)->opds[1] = operand_create_string(opd_read[1]);

        } else if ((*instr)->op->opd_types[1] == NUMBER_TYPE) {
        	(*instr)->opds[1] = operand_create_number(atoi(opd_read[1]));

        }
    }
    if ((*instr)->op->opd_types[2] != OP_NONE) {
	    for (; isspace(s[i]) || s[i] == ','; i++);
	    j = 0;
        while (!(isspace(s[i])) && s[i] != '\0' && s[i] != ',') {
            opd_read[2][j] = s[i];
            j++; i++;
        }
        if ((*instr)->op->opd_types[2] == LABEL) {
        	(*instr)->opds[2] = operand_create_label(opd_read[2]);

        } else if ((*instr)->op->opd_types[2] == REGISTER) {
            (*instr)->opds[2] = operand_create_register(atoi(opd_read[2]));

        } else if ((*instr)->op->opd_types[2] == STRING) {
        	(*instr)->opds[2] = operand_create_string(opd_read[2]);

        } else if ((*instr)->op->opd_types[2] == NUMBER_TYPE) {
        	(*instr)->opds[2] = operand_create_number(atoi(opd_read[2]));

        }
    }
	/*
    for (; s[i] != '\0' || s[i] != '*'; i++)
        if (!(isspace(s[i])) || s[i] != '\n') {
            set_error_msg("Erro de sintaxe");
            *errptr = &(s[i]);
            return 0;
        }
	*/
    return 1;
}
