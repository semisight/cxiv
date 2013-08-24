#include "hash.h"

// Private functions

void clear(cell*, int);

void grow_map(symmap*);

int find_idx_key(symmap*, char*);

int find_idx_empty(symmap*, char*);

void map_put_nogrow(symmap*, char*, obj*);

void clear(cell* arr, int size) {
    for(int i=0; i<size; i++) {
        arr[i].key = NULL;
        arr[i].val = NULL;
    }
}

void grow_map(symmap* m) {
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

int find_idx_key(symmap* m, char* key) {
    int init = hash_str(key) % m->capacity;

    for(int i=0; i < m->capacity; i++) {
        int idx = (init + i) % m->capacity; // Search at init first.

        cell* c = &m->map[idx];

        // If it's empty, then we should have found it by now. It's not here.
        if(!c->key)
            return -1;

        if(strcmp(c->key, key) == 0) {
            return idx;
        }
    }

    // If we get here, we didn't find it. Return -1 as error code.
    return -1;
}

int find_idx_empty(symmap* m, char* key) {
    int idx = hash_str(key) % m->capacity;

    while(m->map[idx].val)
        idx++;

    return idx;
}

void map_put_nogrow(symmap* m, char* key, obj* val) {
    int idx = find_idx_empty(m, key);

    cell* c = &m->map[idx];

    c->key = key;
    c->val = val;

    m->size++;
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

symmap* new_map() {
    symmap* m = malloc(sizeof(symmap));
    if(!m)
        die("Out of memory.");

    m->map = malloc(sizeof(cell) * INITIAL_SIZE);
    if(!m->map)
        die("Out of memory.");

    m->size = 0;
    m->capacity = INITIAL_SIZE;

    // Set vals to NULL for each one. (vals should never be null).
    clear(m->map, m->capacity);

    return m;
}

void map_put(symmap* m, char* key, obj* val) {
    // First, check if we're too full.
    if((double)m->size > (MAX_FILL * m->capacity))
        grow_map(m);

    map_put_nogrow(m, key, val);
}

obj* map_get(symmap* m, char* key) {
    int idx = find_idx_key(m, key);

    // Did we find it?
    if(idx == -1) {
        return NULL;
    }

    return m->map[idx].val;
}

void map_del(symmap* m, char* key) {
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

cell* map_next(symmap* m, map_iter it) {
    while(*it < m->capacity) {
        if(m->map[*it].key)
            return &m->map[(*it)++];

        (*it)++;
    }

    return NULL;
}

int map_contains(symmap* m, char* key) {
    map_iter i = map_start();
    cell* cur;
    while((cur = map_next(m, i))) {
        // Check if we've found a matching key.
        if(strcmp(cur->key, key) == 0)
            return 1;
    }

    // If we find nothing, return false.
    return 0;
}
