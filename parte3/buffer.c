/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: ?
    Raphael dos Reis Gusmao     NUSP: 9778561
*/
#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include "error.h"

/*
  Create and return a new and empty buffer.
*/
Buffer *buffer_create ()
{
    Buffer *B = emalloc(sizeof(Buffer));
    B->data = emalloc(sizeof(char) * 1024);
    B->n = 1024;
    B->i = 0;
    return B;
}

/*
  Destroy a buffer.
*/
void buffer_destroy (Buffer *B)
{
    free(B->data);
    free(B);
}

/*
  Reset buffer, eliminating contents.
*/
void buffer_reset (Buffer *B)
{
    free(B->data);
    B->data = emalloc(sizeof(char) * 1024);
    B->n = 1024;
    B->i = 0;
}

/*
  Add a character c to the end of the buffer.
*/
void buffer_push_back (Buffer *B, char c)
{
    char *temp_data; int b;
    /*
      if buffer is full, allocate more memory
    */
    if (B->i == B->n) {
        B->n *= 2;
        temp_data = emalloc(sizeof(char) * B->n);
        for (b = 0; b < ((B->n)/2); b++) {
            (temp_data[b]) = (B->data[b]);
        }
        free(B->data);
        B->data = temp_data;
    }
    (B->data)[B->i] = c;
    B->i++;
}

/*
  Read a line (i.e., reads up to a newline '\n' character or the
  end-of-file) from the input file and places it into the given
  buffer, including the newline character if it is present. The buffer
  is resetted before the line is read.

  Returns the number of characters read; in particular, returns ZERO
  if end-of-file is reached before any characters are read.
*/
int read_line (FILE *input, Buffer *B)
{
    buffer_reset(B);
    char c;
    c = getc(input);
    while (c != '\n' && c != EOF) {
        buffer_push_back(B, c);
        c = getc(input);
    }
    if (c == '\n')
        buffer_push_back(B, c);
    /*
      signs end of string
    */
    buffer_push_back(B, '\0');
    return (B->i - 1);
}
