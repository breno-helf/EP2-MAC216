#include<stdio.h>
#include<string.h>
#include "stable.h"

int max_size;

int PrintInOrder(const char *key, EntryData *data) {
    int i;
    if(key == NULL || data == NULL) return 0;
    else {
        printf("%s", key);
        for(i = strlen(key); i <= max_size; i++) {
            putchar(' ');
        }
        printf("%d\n", data->i);
        return 1;
    }
}

int main(int argc, char *argv[]) {
    char s[1123];
    SymbolTable table;
    table = stable_create();
    set_prog_name("Symbol Table");
    while(scanf(" %s", s) != EOF) {
        InsertionResult result;
        result = stable_insert(table, s);
        free(result.data);
    }
    stable_visit(table, PrintInOrder);
    stable_destroy(table);
    return 0;
}
