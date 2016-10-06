#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "stable.h"
#include "error.h"

#define TABLE_SIZE 3000017

struct stable_s {
  int *relation;
  char **str;
};

int fexp(int a, int e) {
  
  
}

int get_hash(char *str) {
  
  
  
}

SymbolTable stable_create() {
  int i = 0;
  SymbolTable table;
  table = malloc(sizeof(table));
  table->relation = malloc(TABLE_SIZE*sizeof(int));
  *table->str = malloc(TABLE_SIZE*sizeof(char*));
  for(i = 0; i < TABLE_SIZE; i++) {
    table->relation[i] = 0;
    table->str[i] = NULL;
  }
  return table;
}

void stable_destroy(SymbolTable table) {
  int i;
  free(table->relation);
  for(i = 0; i < TABLE_SIZE; i++)
    free(table->str[i]);
  free(table);
}

InsertionResult stable_insert(SymbolTable table, const char *key) {
  InsertionResult result;
  int s_hash = get_hash(key);
  int hash = s_hash;
  if(table->relation[hash] > 0) {
    while(table->relation[hash] && strcmp(table->str[hash], key)) {
      hash++;
      hash %= TABLE_SIZE;
      if(hash == s_hash) break;
    }
    
    if(table->relation[hash] && !(strcmp(table->str[hash], key))) {
      result.new = 1;
      (*result.data).i = hash;
      return result;
    }
    else if(table->relation[hash] == 0 && (strcmp(table->str[hash], key))) {
      result.new = 0;
      (*result.data).i = hash;
      return result;
    }
    else if(table->relation[hash] && (strcmp(table->str[hash], key))){
      die("Symbol Table is full");
    }
  }
  else {
    result.new = 0;
    (*result.data).i = hash;
    return result;
  }
  die("End of stable_insert without returning value");
}

EntryData *stable_find(SymbolTable table, const char *key) {
  EntryData *result;
  int s_hash = get_hash(key);
  int hash = s_hash;
  if(table->relation[hash] == 0) {
    result = NULL;
    return result;
  }
  else {
    while(table->relation[hash] && strcmp(table->str[hash], key)) {
      hash++;
      hash %= TABLE_SIZE;
      if(hash == s_hash) break;
    }
    if(!(strcmp(table->str[hash], key))) {
      (*result).i = hash;
      return result;
    }
    else if(table->relation[hash] == 0 || strcmp(table->str[hash], key)) {
      result = NULL;
      return result;
    }
  }
  die("End of function stable_find without returning value");
}

int stable_visit(SymbolTable table, int (*visit)(const char *key, EntryData *data)) {





}
