#ifndef HASH_H
#define HASH_H

#define INITIAL_SIZE 16
#define MAX_FILL 0.66

#include <string.h>
#include <stdint.h>

#include "obj.h"

typedef enum {
    SYM,
    OBJ
} map_type;

typedef struct {
    void* key;
    obj* val;
} cell;

typedef struct map_t {
    map_type type;
    cell* map;
    unsigned int size;
    unsigned int capacity;
} map;

typedef map symmap;
typedef map objmap;
typedef unsigned int* map_iter;

uint32_t hash_str(char*);

uint32_t hash_map(map*);

uint32_t hash_obj(obj*);

map* new_map(map_type);

map* map_clone(map*);

void map_put(map*, void*, obj*);

obj* map_get(map*, void*);

//void map_del(map*, void*);

map_iter map_start();

cell* map_next(map*, map_iter);

int map_contains(map*, void*);

#endif