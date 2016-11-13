#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "asmtypes.h"
#include "error.h"

int check_carac (char c) {
/*checa se o caracter é letra, número ou _, devolve 1 se sim, 0 caso contrário*/
    if ((c > 64 && c < 91) || (c == 95) || (c > 96 && c < 123) || (c > 47 && c < 58))
        return 1;
    return 0;
}

int rotulo(char *stg, char *errptr) {
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

int le_str(char *s, char *errptr, SymbolTable table) {
    char *rotulo, *operador;
    Operator *op;
    int i, j = 0, a;
    rotulo = malloc(sizeof(char) * 20);
    for(i = 0; s[i] == ' '; i++);
    while (s[i] != ' ' && s[i] != '\0') {
        rotulo[j] = s[i];
        j++; i++;
    }
    rotulo[j] = '\0';
    if (rotulo(rotulo, errptr) == 0) {      
		printf("Rotulo %s invalido\n", rotulo);
        return 0;
    }

	if((op = optable_find(rotulo)) != 0) {
        instr->op = op; /* A funcao optable_find ja faz isso */
		rotulo = NULL;		
	}
	if(stable_find(table, rotulo) != NULL) {
		printf("Rotulo %s ja existe\n", rotulo);
		errptr = rotulo;
	}

	instr->label = rotulo;
    operador = malloc(sizeof(char) * 16);
    for(; s[i] == ' '; i++);
    j = 0;
    while (s[i] != ' ' && s[i] != '\0') {
        operador[j] = s[i];
        j++; i++;
    }

	if((op = optable_find(operador)) != 0) { /* Aqui nao sei se eh
												Operador ou rotulo
											 */
        instr->op = op;
		rotulo = NULL;
	}
			
	else {
		printf("%s Nao eh operador\n", operador); /* Arruma aqui dps
													 de arrumar ali
													 em cima
												  */
		errptr = operador;
	}
}
	
int parse(const char *s, SymbolTable alias_table, Instruction **instr, const char **errptr) {
	/*
	  Aqui fica o monstro
	*/		
}










