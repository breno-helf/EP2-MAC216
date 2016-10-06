#include <stdio.h>

// Buffer struct
typedef struct buffer_s {
  char *data;

  // Buffer max. size and first free position.
  int n, i;
} Buffer;

/*
  Create and return a new and empty buffer.
*/
Buffer *buffer_create() {
    Buffer *B;
    B->
    B->n = 0
    B->i = 0
    return B;
}

/*
  Destroy a buffer.
*/
void buffer_destroy(Buffer *B) {
    free(B->data);
    free(B);
}

/*
  Reset buffer, eliminating contents.
*/
void buffer_reset(Buffer *B) {
    free(B->data);
    B->n = 0
    B->i = 0
}

/*
  Add a character c to the end of the buffer.
*/
void buffer_push_back(Buffer *B, char c) {
    if (i == n) {
        //malloc dobra n
    }
}

/*
  Read a line (i.e., reads up to a newline '\n' character or the
  end-of-file) from the input file and places it into the given
  buffer, including the newline character if it is present. The buffer
  is resetted before the line is read.

  Returns the number of characters read; in particular, returns ZERO
  if end-of-file is reached before any characters are read.
*/
int read_line(FILE *input, Buffer *B) {
    buffer_reset(B);
    while (!(\n or EOF))
        buffer_push_back(B, c);
    if (\n)
        buffer_push_back(B, c);
    return(B->i);
}