typedef union {
  int i;
  char *str;
  void *p;
} EntryData;

typedef struct stable_s SymbolTable;

struct stable_s {

	EntryData data;
	SymbolTable *left;
	SymbolTable *right;
};

#include "ss.c"

typedef struct {
  int new;
  EntryData *data;
} InsertionResult;

SymbolTable* stable_create();

SymbolTable* stable_find(SymbolTable *st, const char *key);

SymbolTable* stable_insert(SymbolTable *st, const char *key);

void printInOrder(SymbolTable *st);
