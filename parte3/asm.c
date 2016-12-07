/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: ?
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

	/*
	  Após isso percorra a lista gerando o código objeto de cada
	  instrução, definido por nós mesmos :P
	*/
}
