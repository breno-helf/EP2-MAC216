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

char* HexToDec (char *s)
{
	char *ret;
	int num = 0, tam = 0, i, pow = 1, aux;
	ret = malloc(15);
	while (s[tam] != 0) tam++;

	for (i = tam - 1; i > 0; i--) {

		if (isdigit(s[i]))
			num += pow*(s[i] - '0');
		else if (islower(s[i]))
			num += pow*(s[i] - 'a' + 10);
		else
			num += pow*(s[i] - 'A' + 10);
		pow *= 16;
	}
	tam = 0;
	aux = num;
	while (aux) {
		aux /= 10;
		tam++;
	}

	for (i = tam - 1; i >= 0; i--) {
		ret[i] = '0' + (num%10);
		num /= 10;
	}
	return ret;
}


int parse (const char *s, SymbolTable alias_table, Instruction **instr, const char **errptr) {
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
	} else {
		set_error_msg("Nao eh operador");
		*errptr = operador;
		return 0;
	}

 operando:
	for (; isspace(s[i]); i++);

    if ((*instr)->op->opd_types[0] != OP_NONE) {
		EntryData *col;
		j = 0;

        while (!(isspace(s[i])) && s[i] != '\0' && s[i] != ',') {
            opd_read[0][j] = s[i];
            j++; i++;
        }

		col = stable_find(alias_table, opd_read[0]);
		if (col) {
			(*instr)->opds[0] = col->opd;
		} else if (((*instr)->op->opd_types[0]&LABEL) == LABEL) {
        	(*instr)->opds[0] = operand_create_label(opd_read[0]);
        } else if (!(isdigit(opd_read[0][0]) || opd_read[0][0] == '#') && ((*instr)->op->opd_types[0]&REGISTER) == REGISTER) {
			int k = 0;
			while(!(isdigit(opd_read[0][k]))) k++;
            (*instr)->opds[0] = operand_create_register(atoi(&opd_read[0][k]));
        } else if (((*instr)->op->opd_types[0]&STRING) == STRING) {
        	(*instr)->opds[0] = operand_create_string(opd_read[0]);
        } else {
			if (opd_read[0][0] == '#')
				opd_read[0] = HexToDec(opd_read[0]);
        	(*instr)->opds[0] = operand_create_number(atoi(&opd_read[0][0]));
        }
    }

    if ((*instr)->op->opd_types[1] != OP_NONE) {
		EntryData *col;
	    for (; isspace(s[i]) || s[i] == ','; i++);
	    j = 0;
        while (!(isspace(s[i])) && s[i] != '\0' && s[i] != ',') {
            opd_read[1][j] = s[i];
            j++; i++;
        }
		col = stable_find(alias_table, opd_read[1]);
		if (col) {
			(*instr)->opds[1] = col->opd;
		}
        else if (((*instr)->op->opd_types[1]&LABEL) == LABEL) {
        	(*instr)->opds[1] = operand_create_label(opd_read[1]);
        } else if (!(isdigit(opd_read[1][0]) || opd_read[1][0] == '#') && ((*instr)->op->opd_types[1]&REGISTER) == REGISTER) {
			int k = 0;
			while(!(isdigit(opd_read[1][k]))) k++;
            (*instr)->opds[1] = operand_create_register(atoi(&opd_read[1][k]));
        } else if (((*instr)->op->opd_types[1]&STRING) == STRING) {
        	(*instr)->opds[1] = operand_create_string(opd_read[1]);
        } else {
			if(opd_read[1][0] == '#')
				opd_read[1] = HexToDec(opd_read[1]);
        	(*instr)->opds[1] = operand_create_number(atoi(&opd_read[1][0]));
        }
    }

    if ((*instr)->op->opd_types[2] != OP_NONE) {
		EntryData *col;
	    for (; isspace(s[i]) || s[i] == ','; i++);

	    j = 0;
        while (!(isspace(s[i])) && s[i] != '\0' && s[i] != ',') {
            opd_read[2][j] = s[i];
            j++; i++;
        }

		col = stable_find(alias_table, opd_read[2]);
		if (col) {
			(*instr)->opds[2] = col->opd;
		} else if (((*instr)->op->opd_types[2]&LABEL) == LABEL) {
        	(*instr)->opds[2] = operand_create_label(opd_read[2]);
        } else if (!(isdigit(opd_read[2][0]) || opd_read[2][0] == '#') && ((*instr)->op->opd_types[2]&REGISTER) == REGISTER) {
            printf("*1\n");/*--------------------------------------------TODO */
			int k = 0;
			while(!(isdigit(opd_read[2][k]))) {
                k++;
            }
            printf("*2\n");/*--------------------------------------------TODO */
            (*instr)->opds[2] = operand_create_register(atoi(&opd_read[2][k]));
        } else if (((*instr)->op->opd_types[2]&STRING) == STRING) {
        	(*instr)->opds[2] = operand_create_string(opd_read[2]);
        } else {
			if(opd_read[2][0] == '#')
				opd_read[2] = HexToDec(opd_read[2]);
        	(*instr)->opds[2] = operand_create_number(atoi(&opd_read[2][0]));
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
