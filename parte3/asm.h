/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: ?
    Raphael dos Reis Gusmao     NUSP: 9778561
*/
#include "asmtypes.h"
#include "stable.h"
#include <stdio.h>
#include <stdlib.h>

/*
    Verifica se a instrucao eh um codigo
    (diferente de EXTERN e IS)
*/
int isCode (Instruction *instr);

/*
    Verifica se a instrucao eh um pseudo codigo
    (CALL, PUSH ou RET)
*/
int isPseudoCode (Instruction *instr);

/*
    Converte uma instrucao em pseudo codigo para codigo normal
*/
Instruction* pseudoToCode (Instruction *instr);

/*
    Altera o valor da variavel globalOut
*/
void set_globalOut (FILE *output);

/*
    Imprime o label e sua key no arquivo de saida
*/
int outPrint (const char *key, EntryData *data);

/*
    Faz a montagem do programa
*/
int assemble (const char *filename, FILE *input, FILE *output);
