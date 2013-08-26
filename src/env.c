#include "cxiv.h"
#include "env.h"

// Private functions

map* env_find(env* in, char* key);

map* env_find(env* in, char* key) {
    obj* walk = in;
    map* cur;

    if(in->type != PAIR)
        die("Env find failed: outside of any scope.");

    while(walk->type == PAIR) {
        cur = car(walk)->map_value;

        if(map_contains(cur, key))
            return cur;

        walk = cdr(walk);
    }

    return NULL;
}

// Public functions

env* new_env() {
    return env_extend(val_nil);
}

env* env_extend(env* in) {

    // Create a new map, but change it to SYM type.
    obj* m = new_omap();
    m->map_value->type = SYM;

    return cons(m, in);
}

env* env_outer(env* in) {
    return cdr(in);
}

void env_define(env* in, char* key, obj* val) {

    // If nil, die.
    if(in->type != PAIR)
        die("Define failed: outside of any scope.");

    map* scp = car(in)->map_value;

    map_put(scp, key, val);
}

obj* env_get(env* in, char* key) {
    map* scp = env_find(in, key);

    if(!scp)
        die("Variable unbound.");

    return map_get(scp, key);
}

void env_set(env* in, char* key, obj* val) {
    map* scp = env_find(in, key);

    if(!scp)
        die("Variable unbound.");

    return map_put(scp, key, val);
}
