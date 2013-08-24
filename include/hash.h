#ifndef HASH_H
#define HASH_H

#define INITIAL_SIZE 16
#define MAX_FILL 0.66

#include <string.h>
#include <stdint.h>

#include "obj.h"
#include "util.h"

typedef struct {
    char* key;
    obj* val;
} cell;

typedef struct {
    cell* map;
    unsigned int size;
    unsigned int capacity;
} symmap;

typedef unsigned int* map_iter;

uint32_t hash_str(char*);

symmap* new_map();

void map_put(symmap*, char*, obj*);

obj* map_get(symmap*, char*);

void map_del(symmap*, char*);

map_iter map_start();

cell* map_next(symmap*, map_iter);

int map_contains(symmap*, char*);

#endif