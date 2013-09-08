#include "cxiv.h"
#include "obj.h"
#include "hash.h"

// Private functions

void clear(cell*, int);

void grow_map(map*);

int find_idx_key(map*, void*);

int find_idx_empty(map*, void*);

void map_put_nogrow(map*, void*, obj*);

int cmpkey(map*, void*, void*);

void clear(cell* arr, int size) {
    for(int i=0; i<size; i++) {
        arr[i].key = NULL;
        arr[i].val = NULL;
    }
}

void grow_map(map* m) {
    // Strategy: save original cell array, then create a new blank one.
    // Iterate through the old one and repopulate the new one.

    // Save
    cell* old = m->map;
    int ocap = m->capacity;

    // Create
    m->size = 0;
    m->capacity *= 2;
    m->map = malloc(sizeof(cell) * m->capacity);

    clear(m->map, m->capacity);

    // Repopulate
    for(int i=0; i<ocap; i++) {
        if(!old[i].key)
            continue;

        map_put_nogrow(m, old[i].key, old[i].val);
    }

    // Cleanup
    free(old);
}

int find_idx_key(map* m, void* key) {
    int init = m->type == SYM ? hash_str(key) : hash_obj(key);
    init = init % m->capacity;

    for(int i=0; i < m->capacity; i++) {
        int idx = (init + i) % m->capacity; // Search at init first.

        cell* c = &m->map[idx];

        // If it's empty, then we should have found it by now. It's not here.
        if(!c->key)
            return -1;

        if(cmpkey(m, c->key, key))
            return idx;

    }

    // If we get here, we didn't find it. Return -1 as error code.
    return -1;
}

int find_idx_empty(map* m, void* key) {
    int init = m->type == SYM ? hash_str(key) : hash_obj(key);
    init = init % m->capacity;

    for(int i=0; i < m->capacity; i++) {
        int idx = (init + i) % m->capacity; // Search at init first.

        cell* c = &m->map[idx];

        if(!c->key)
            return idx;
    }

    // If we get here, we didn't find any space (impossible).
    die("Illegal state. Could not find space in map.");
    return -1;
}

void map_put_nogrow(map* m, void* key, obj* val) {
    int idx = find_idx_key(m, key); // See if it exists first.
    
    // Otherwise create it.
    if(idx == -1)
        idx = find_idx_empty(m, key);

    cell* c = &m->map[idx];

    c->key = key;
    c->val = val;

    m->size++;
}

int cmpkey(map* m, void* a, void* b) {
    if(m->type == SYM) {
        return strcmp(a, b) == 0;
    } else {
        return is_equal(a, b);
    }
}

// Public functions

// djb2 hash function adapted from http://www.cse.yorku.ca/~oz/hash.html.
uint32_t hash_str(char* str) {
    uint32_t hash = 5381;
    int c;

    while((c = *str++))
        hash = (hash << 5) + hash + c;

    return hash;
}

uint32_t hash_map(map* in) {
    map_iter i = map_start();
    cell* cur;
    uint32_t hash = 0;

    while((cur = map_next(in, i))) {
        hash += hash_obj(cur->key) + hash_obj(cur->val);
    }

    return hash;
}

uint32_t hash_obj(obj* in) {
    switch(in->type) {
    case NUMBER:
        return (uint32_t)in->num_value;
    case BOOL:
        return (uint32_t)in;
    case CHAR:
        return (uint32_t)in->char_value;
    case STRING:
        return hash_str(in->string_value);
    case NIL:
        return 42; // Because why not?
    case PAIR:
        return hash_obj(car(in)) + hash_obj(cdr(in));
    case SYMBOL:
        return hash_str(in->symbol_value);
    case MAP:
        return hash_map(in->map_value);
    case PROC_NATIVE:
        return (uint32_t)in->proc_native.call; // Use function address.
    case PROC_COMPOUND:
        return (uint32_t)in;
    }
}

map* new_map(map_type t) {
    map* m = malloc(sizeof(map));
    if(!m)
        die("Out of memory.");

    m->map = malloc(sizeof(cell) * INITIAL_SIZE);
    if(!m->map)
        die("Out of memory.");

    m->size = 0;
    m->capacity = INITIAL_SIZE;
    m->type = t;

    // Set vals to NULL for each one. (vals should never be null).
    clear(m->map, m->capacity);

    return m;
}

map* map_clone(map* in) {
    // Copy all elements of the input map.
    map* rv = new_map(in->type);
    map_iter i = map_start();
    cell* cur;

    while((cur = map_next(in, i)))
        map_put(rv, cur->key, cur->val);

    return rv;
}

void map_put(map* m, void* key, obj* val) {
    // First, check if we're too full.
    if((double)m->size > (MAX_FILL * m->capacity))
        grow_map(m);

    map_put_nogrow(m, key, val);
}

obj* map_get(map* m, void* key) {
    int idx = find_idx_key(m, key);

    // Did we find it?
    if(idx == -1) {
        return NULL;
    }

    return m->map[idx].val;
}

void map_del(map* m, void* key) {
    int idx = find_idx_key(m, key);

    // Should we free key/val??
    cell* c = &m->map[idx];

    c->key = NULL;
    c->val = NULL;
}

map_iter map_start() {
    map_iter it = malloc(sizeof(map_iter));
    *it = 0;

    return it;
}

cell* map_next(map* m, map_iter it) {
    while(*it < m->capacity) {
        if(m->map[*it].key)
            return &m->map[(*it)++];

        (*it)++;
    }

    return NULL;
}

int map_contains(map* m, void* key) {
    map_iter i = map_start();
    cell* cur;
    while((cur = map_next(m, i))) {
        // Check if we've found a matching key.
        if(cmpkey(m, cur->key, key))
            return true;
    }

    // If we find nothing, return false.
    return false;
}
