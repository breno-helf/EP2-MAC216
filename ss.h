/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: ?
    Raphael dos Reis Gusmao     NUSP: 9778561
*/
#ifndef __SS_H__
#define __SS_H__

#include "ss.c"

typedef union
{
  int i;
  char *str;
  void *p;
} EntryData;

typedef struct stable_s SymbolTable;

struct stable_s
{
	EntryData data;
	SymbolTable *left;
	SymbolTable *right;
};

typedef struct
{
  int new;
  EntryData *data;
} InsertionResult;

SymbolTable* stable_create ();

SymbolTable* stable_find (SymbolTable *st, const char *key);

SymbolTable* stable_insert (SymbolTable *st, const char *key);

void printInOrder (SymbolTable *st);

#endif
