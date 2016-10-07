#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "stable.h"
#include "buffer.h"
#include "error.h"
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
    SymbolTable table;
    FILE *in;
    Buffer *buffer = buffer_create();
    set_prog_name("Symbol Table");    
    if(argc != 2)
        die("Numero de argumentos errados");
        
    in = fopen(argv[1], "r");

    table = stable_create();

    while(read_line(in, buffer)) {
        InsertionResult result;
        int i, j, size;
        char *word;
        word = malloc(sizeof(char)*(buffer->n));
        if(word == NULL) die("!Out of memory");
        
        for(i = 0; buffer->data[i] != '\0'; i++) {
            size = 0;
            if(!isspace(buffer->data[i])) {
                for(j = i; buffer->data[j] != '\0' && !(isspace(buffer->data[j])); j++) {
                    word[size++] = buffer->data[j];
                }
                word[size++] = '\0';
                i = j;
                result = stable_insert(table, word);
                free(result.data);
            }
        }
    }
    stable_visit(table, PrintInOrder);
    //stable_destroy(table);
    return 0;
}
