#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "stable.h"
#include "error.h"

#define TABLE_SIZE 20011

/*
  struct stable_s, it should store the
  symbol table. elements is a vector that stores
  the hashing values of the elements inserted,
  and relation and str are the pair (k, i) stored
  in the position of whose value is the hashing of k.
*/
struct stable_s {
    int *relation;
    int *elements;
    char **str;
    int size;
};

/*
  global variable SymbolTable st used
  in purpose to sort array.
*/
SymbolTable st;
int max_size;

/*
  get_hash function, it should receive a string str and
  return the its hashing MOD 20011. The hashing is polinomial
  and the prime used to hash is 131.
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

/*
  Creates a new symbol table and return it
*/
SymbolTable stable_create() {
    int i = 0;
    SymbolTable table;
    table = malloc(sizeof(struct stable_s));
    if(table == NULL) die("!Out of memory");

    table->relation = malloc((TABLE_SIZE + 5)*sizeof(int));
    if(table->relation == NULL) die("!Out of memory");
    
    table->str = malloc((TABLE_SIZE + 5)*sizeof(char*));
    if(table->str == NULL) die("!Out of memory");
    
    table->size = 0;
    table->elements = malloc((TABLE_SIZE + 5)*sizeof(int));
    if(table->elements == NULL) die("!Out of memory");
    
    for(i = 0; i <= TABLE_SIZE; i++) {
        table->relation[i] = 0;
        table->str[i] = NULL;
    }
    return table;
}

/*
  Destroy a given symbol table
*/
void stable_destroy(SymbolTable table) {
    int i;
    if(table->relation != NULL) free(table->relation);
    if(table->elements != NULL) free(table->elements);
    for(i = 0; i < TABLE_SIZE; i++) {
        if(table->str[i] == NULL) continue;
        free(table->str[i]);
    }
    if(table->str != NULL) free(table->str);
    if(table != NULL) free(table);
}

/*
  Insert a new entry on the symbol table given its key.
  
  To insert an entry it hash de string and inserts using linear
  probing.

  If there is already an entry with the given key, then a struct
  InsertionResult is returned with new == 0 and data pointing to the
  data associated with the entry. Otherwise, a struct is returned with
  new != 0 and data pointing to the data field of the new entry.

  If there is not enough space on the table, or if there is a memory
  allocation error, then crashes with an error message.
*/
InsertionResult stable_insert(SymbolTable table, const char *key) {
    InsertionResult result;
    result.data = malloc(sizeof(EntryData));
    if(result.data == NULL) die("!Out of memory");
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
            table->str[hash] = malloc((size + 2)*sizeof(char));
            if(table->str[hash] == NULL) die("!Out of memory!");
            
            for(i = 0; i <= size; i++)
                table->str[hash][i] = key[i];

            result.new = 1;
            result.data->i = ++table->relation[hash];
            table->elements[table->size++] = hash;
            return result;
        }
        else if(table->relation[hash] && (strcmp(table->str[hash], key) == 0)) {
            result.new = 0;
            result.data->i = hash;
            ++table->relation[hash];
            return result;
        }
        else if(table->relation[hash] && strcmp(table->str[hash], key))
            die("Symbol Table is full");
    }
    else {
        int size = strlen(key), i;
        table->str[hash] = malloc((size + 2)*sizeof(char));
        if(table->str[hash] == NULL) die("!Out of memory!");
        
        for(i = 0; i <= size; i++)
            table->str[hash][i] = key[i];

        result.new = 1;
        result.data->i = ++table->relation[hash];
        table->elements[table->size++] = hash;
        return result;
    }
    die("End of stable_insert without returning value");
}

/*
  Find the data associated with a given key.

  Given a key, returns a pointer to the data associated with it, or a
  NULL pointer if the key is not found.
  To find a data associated with a key, it uses hashing aligned with
  linear probing.
*/
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
            if(result == NULL) die("!Out of memory");
            
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

/*
  order funcition, used to sort the elements array lexicographically
  by the string that each number in elements represent. To represent
  the string it is used polynomial hashing with linear probing.
*/
int order(const void *a, const void *b) {
    return strcmp(st->str[*(int*)a], st->str[*(int*)b]);
}

/*
  Visit each entry on the table.

  The visit function is called on each entry, with pointers to its key
  and data. If the visit function returns zero, then the iteration
  stops.

  Returns zero if the iteration was stopped by the visit function,
  nonzero otherwise.
*/
int stable_visit(SymbolTable table, int (*visit)(const char *key, EntryData *data)) {
    int i = 0;
    EntryData *entry;
    entry = malloc(sizeof(EntryData));
    st = table;
    qsort(table->elements, table->size, sizeof(int), order);
    max_size = 0;
    for(i = 0; i < table->size; i++) {
        if(strlen(table->str[table->elements[i]]) > max_size)
            max_size = strlen(table->str[table->elements[i]]);
    }
    i = 0;
    while(i < table->size){
        entry->i = table->relation[table->elements[i]];
        if(!(visit(table->str[table->elements[i]], entry))) {
            return 0;
        }
        i++;
    }
    return 1;
}
