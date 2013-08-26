#ifndef PRINT_H
#define PRINT_H

#include "obj.h"

/* 
 * print.h
 *
 * Prints an object tree.
 */

void print_list(obj* in);

void print_map(obj* in);

void print(obj* in);

#endif
