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

int is_delim(int c);

void consume_whitespace(FILE*);

int peek(FILE*);

obj* read(FILE*);

#endif
