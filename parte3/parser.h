/*
    parser.h

    Parsing routines for the assembly language.
*/
#ifndef __PARSER_H__
#define __PARSER_H__

#include "stable.h"
#include "asmtypes.h"

/*
    Verifica se o caracter eh letra, numero ou _
*/
int isCharValid (char c);

/*
    Verifica se o rotulo eh valido_
*/
int checkRotulo (char *str, const char *errptr);

/*
    Converte Hexadecimal para Decimal
*/
char* HexToDec (char *s);

/*
    Return instruction corresponding to assembly language line.

    INPUT:

    - s -- line of assembly code.

    - alias_table -- table of aliases used so far.

    Returns nonzero on success, zero to signal an error.  On  success,
    *instr contains the parsed instruction, unless the line was empty.
    On error, *errptr, if non-NULL, points to the character in s where
    the error was found.
*/
int parse(const char *s, SymbolTable alias_table, Instruction **instr, const char **errptr);

#endif
