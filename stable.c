#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "stable.h"
#include "error.h"

#define TABLE_SIZE 30011

struct stable_s {
    char **str;
    int *relation;
    int size;
    int *elements;
};

/*
  get_hash function, it should receive a string str and
  return the its hashing MOD 3000017. The prime used to
  hash is 131.
*/
int get_hash(const char *s) {
    int i, ret = 0, p = 131, pow = 1;
    for(i = 0; s[i] != '\0'; i++) {
        ret += (s[i])*pow;
        pow *= p;
        pow %= TABLE_SIZE;
        ret %= TABLE_SIZE;
    }
    return ret;
}

SymbolTable stable_create() {
    int i = 0;
    SymbolTable table;
    table = malloc(sizeof(table));
    table->relation = malloc(TABLE_SIZE*sizeof(int));
    table->str = malloc(TABLE_SIZE*sizeof(char*));
    table->size = 0;
    table->elements = malloc(TABLE_SIZE*sizeof(int));
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
    free(table->str);
    free(table);
}

InsertionResult stable_insert(SymbolTable table, const char *key) {
    InsertionResult result;
    result.data = malloc(sizeof(EntryData));
    if(result.data == NULL)
        die("!Out of memory");
    int s_hash = get_hash(key);
    int hash = s_hash;
    if(table->relation[hash] > 0) {
        while(table->relation[hash] && strcmp(table->str[hash], key)) {
            hash++;
            hash %= TABLE_SIZE;
            if(hash == s_hash) break;
        }
        if(table->relation[hash] == 0) {
            int size = strlen(key), i;
            table->str[hash] = malloc(size*sizeof(char));
            if(table->str[hash] == NULL)
                die("!Out of memory!");
            for(i = 0; i < size; i++)
                table->str[hash][i] = key[i];
            result.new = 1;
            result.data->i = ++table->relation[hash];
            table->elements[++table->size] = hash;
            return result;
        }
        else if(table->relation[hash] && (strcmp(table->str[hash], key) == 0)) {
            result.new = 0;
            result.data->i = hash;
            ++table->relation[hash];
            return result;
        }
        else if(table->relation[hash] && (strcmp(table->str[hash], key))){
            die("Symbol Table is full");
        }
    }
    else {
        int size = strlen(key), i;
        table->str[hash] = malloc(size*sizeof(char));
        if(table->str[hash] == NULL)
            die("!Out of memory!");
        for(i = 0; i < size; i++)
            table->str[hash][i] = key[i];
        result.new = 1;
        result.data->i = ++table->relation[hash];
        table->elements[++table->size] = hash;
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
        if((table->str[hash] != NULL) && !(strcmp(table->str[hash], key))) {
            result = malloc(sizeof(EntryData));
            if(result == NULL)
                die("!Out of memory");
            result->i = table->relation[hash]; 
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
