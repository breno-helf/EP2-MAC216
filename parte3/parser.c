/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: 8991769
    Raphael dos Reis Gusmao     NUSP: 9778561
*/
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include "asmtypes.h"
#include "error.h"
#include "stable.h"
#include "optable.h"

int isCharValid (char c)
{
    return ((c >= 'A' && c <= 'Z') || (c == '_') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'));
}

int checkRotulo (char *str, const char *errptr)
{
    int i = 1;
    if (!(isalpha(str[0]) || str[0] == '_')) {
        errptr = &str[0];
		set_error_msg("primeiro char nao eh letra ou underscore");
		return 0;
    }
    while (str[i] != '\0') {
        if (!isCharValid(str[i])) {
			errptr = &str[i];
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
	ret = emalloc(15);
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
    char *rotulo, *operador, **opd_read;
    int i, j, opnum;
    const Operator *op;
    EntryData *data;

    /* Guarda em rotulo a primeira palavra encontrada */
    rotulo = emalloc(sizeof(char) * 16);
    for (i = 0; isspace(s[i]); i++);
    j = 0;
    while (!(isspace(s[i])) && s[i] != '\0' && j < 16) {
        rotulo[j] = s[i];
        j++; i++;
    }
    rotulo[j] = '\0';

    /* Verifica se o rotulo tem tamanho valido */
    if (j == 16) {
        set_error_msg("Rotulo invalido");
        *errptr = rotulo;
        return 0;
    }

    /* Verifica se o rotulo eh valido */
    if (!checkRotulo(rotulo, *errptr)) {
		set_error_msg("Rotulo invalido");
        return 0;
    }

    /* Verifica se o rotulo eh na verdade um operador*/
	if ((op = optable_find(rotulo))) {
        (*instr)->op = op;
		rotulo = NULL;
	} else {
        /* Verifica se o rotulo ja existe */
    	if (stable_find(alias_table, rotulo) != NULL) {
            set_error_msg("Rotulo ja existe");
            *errptr = rotulo;
            return 0;
        }

        /* Rotulo valido*/
        (*instr)->label = rotulo;

        /* Guarda em operador a proxima palavra encontrada */
        operador = emalloc(sizeof(char) * 16);
        for (; isspace(s[i]); i++);
        j = 0;
        while (!(isspace(s[i])) && s[i] != '\0') {
            operador[j] = s[i];
            j++; i++;
        }
        operador[j] = '\0';

        /* Verifica se eh um operador */
    	if (!(op = optable_find(operador))) {
            set_error_msg("Nao eh operador");
    		*errptr = operador;
    		return 0;
    	}

        /* Operador valido*/
        (*instr)->op = op;
    }

    /* Percorre os possiveis operandos da instrucao */
    opd_read = emalloc(3 * sizeof(char *));
    for (opnum = 0; opnum < 3; opnum++) {
        if ((*instr)->op->opd_types[opnum] != OP_NONE) {
            /* Guarda em opd_read[opnum] a proxima palavra encontrada */
            opd_read[opnum] = emalloc(sizeof(char) * 16);
            if (!opnum) for (; isspace(s[i]); i++);
            else for (; isspace(s[i]) || s[i] == ','; i++);
            j = 0;
            while (!(isspace(s[i])) && s[i] != '\0' && s[i] != ',' && j < 16) {
                opd_read[opnum][j] = s[i];
                j++; i++;
            }
            opd_read[opnum][j] = '\0';

            /* Verifica se o operando tem tamanho valido */
            if (j == 0) {
                set_error_msg("Operando faltando");
                *errptr = &opd_read[opnum];
                return 0;
            }

            /* Verifica se o operando tem tamanho valido */
            if (j == 16) {
                set_error_msg("Operando invalido");
                *errptr = &opd_read[opnum];
                return 0;
            }

            /* Verifica se o operando esta na tabela de alias */
            data = stable_find(alias_table, opd_read[opnum]);

            /* Se o operando lido for um alias */
            if (data) {
                (*instr)->opds[opnum] = data->opd;
            }
            /* Se o operando esperado for um label */
            else if (((*instr)->op->opd_types[opnum] & LABEL) == LABEL) {
                (*instr)->opds[opnum] = operand_create_label(opd_read[opnum]);
            }
            /* Se o operando for um registrador*/
            else if (((*instr)->op->opd_types[opnum] & REGISTER) == REGISTER) {
                if(!(isdigit(opd_read[opnum][0]) || opd_read[opnum][0] == '#')) {
                    int k = 0;
                    while(!isdigit(opd_read[opnum][k]) && k < 16) k++;
                    if (k == 16) {
                        set_error_msg("Operando invalido");
                        *errptr = &opd_read[opnum];
                        return 0;
                    }
                    (*instr)->opds[opnum] = operand_create_register(atoi(&opd_read[opnum][k]));
                }
            }
            /* Se o operando esperado for uma string */
            else if (((*instr)->op->opd_types[opnum] & STRING) == STRING) {
                (*instr)->opds[opnum] = operand_create_string(opd_read[opnum]);
            }
            /* Se o operando for um numero hexadecimal */
            else {
                if (opd_read[opnum][0] == '#')
                    opd_read[opnum] = HexToDec(opd_read[opnum]);
                (*instr)->opds[opnum] = operand_create_number(atoi(&opd_read[opnum][0]));
            }
        }
    }

    return 1;
}
