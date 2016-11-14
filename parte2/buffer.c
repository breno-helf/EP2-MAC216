/*
    Breno Helfstein Moura       NUSP: 9790972
    Lucas Daher                 NUSP: 8991769
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
    Buffer *buffer;
    buffer = malloc(sizeof(Buffer));
    if (buffer == NULL) {
        printf("Out of memory!\n");
        exit(1);
    }
    buffer->data = malloc(sizeof(char) * 1024);
    if (buffer->data == NULL) {
        printf("Out of memory!\n");
        exit(1);
    }
    buffer->n = 1024;
    buffer->i = 0;
    return buffer;
}

/*
  Destroy a buffer.
*/
void buffer_destroy (Buffer *buffer)
{
    free(buffer->data);
    free(buffer);
}

/*
  Reset buffer, eliminating contents.
*/
void buffer_reset (Buffer *buffer)
{
    free(buffer->data);
    buffer->data = malloc(sizeof(char) * 1024);
    if (buffer->data == NULL) {
        printf("Out of memory!\n");
        exit(1);
    }
    buffer->n = 1024;
    buffer->i = 0;
}

/*
  Add a character c to the end of the buffer.
*/
void buffer_push_back (Buffer *buffer, char c)
{
    if (buffer->i == buffer->n) {
        Buffer *newBuffer;
        newBuffer = malloc(sizeof(Buffer) * 2 * buffer->n);
        if (newBuffer == NULL) {
            printf("Out of memory!\n");
            exit(1);
        }
        int j;
        for (j = 0; j < buffer->i; j++)
            newBuffer->data[j] = buffer->data[j];
        newBuffer->i = j;
        newBuffer->n = 2 * buffer->n;
        buffer_destroy(buffer);
        buffer = newBuffer;
    }
    buffer->data[buffer->i++] = c;
}

/*
  Read a line (i.e., reads up to a newline '\n' character or the
  end-of-file) from the input file and places it into the given
  buffer, including the newline character if it is present. The buffer
  is resetted before the line is read.

  Returns the number of characters read; in particular, returns ZERO
  if end-of-file is reached before any characters are read.
*/
int read_line (FILE *input, Buffer *buffer)
{
    buffer_reset(buffer);
    char c = getc(input);
    while (c != '\n' && c != EOF && c != ';' && c != '*') {
        buffer_push_back(buffer, c);
        c = getc(input);
    }
    if (c == '*') do {
        c = getc(input);
    } while (c != '\n');
    buffer_push_back(buffer, '\0');
    if (buffer->i == 1) {
        if (c == EOF)
            return -1;
        return 0;
    }
    return (buffer->i - 1);
}
