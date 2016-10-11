/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: ?
    Raphael dos Reis Gusmao     NUSP: 9778561
*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "error.h"
#include "buffer.h"

int main (int argc, char *argv[])
{
    set_prog_name("center");
    
    int c, line = 0;
    FILE *in, *out;
    Buffer *buffer = buffer_create();
    
    /* Recebe os argumentos da linha de comando */
    if (argc != 4)
        die("Numero de argumentos errado.");
    c = atoi(argv[3]);
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    
    /* Le cada linha do arquivo de entrada */
    line = 0;
    while (read_line(in, buffer)) {
        int ini = 0, fim = (buffer->i)-2, colFaltam, i;
        char *str = buffer->data;
        line++;
        
        /* Encontra o primeiro e o (ultimo + 1) char nao-espaco */
        while (ini < fim && isspace(str[ini]))  ini++;
        while (ini < fim && isspace(str[fim-1]))  fim--;
        
        /* Colunas que faltam para preencher a linha */
        colFaltam = c - (fim - ini);
        
        /* Imprime a linha centralizada */
        if (colFaltam != c) {
            if (colFaltam % 2 == 0)
                for (i = 0; i < colFaltam/2; i++)  fprintf(out, " ");
            else
                for (i = 0; i <= colFaltam/2; i++)  fprintf(out, " ");
            
            for (i = ini; i < fim; i++)  fprintf(out, "%c", str[i]);
            for (i = 0; i < colFaltam/2; i++)  fprintf(out, " ");
            
            if (colFaltam < 0)
                print_error_msg("%s: line %d: line too long.", argv[1], line);
        }
        fprintf(out, "\n");
    }
    
    // Fecha os arquivos
    fclose(in);
    fclose(out);
    
    return 0;
}
