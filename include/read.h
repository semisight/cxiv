#ifndef READ_H
#define READ_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "obj.h"

/* 
 * read.h
 *
 * Functions involved in reading from a file (pretty much anything that takes a
 * file pointer.
 */

// Initial string read size. Will grow if necessary.
#define BUFFER_INIT 8

// Predicates

int is_delim(int c);

// Stream processors

void consume_whitespace(FILE*);

void consume_expected(FILE*, const char*);

int peek(FILE*);

double read_num(char, FILE*);

int read_bool(char, FILE*);

char read_char(char, FILE*);

char* read_string(char, FILE*);

obj* read_pair(FILE*);

obj* read(FILE*);

#endif
