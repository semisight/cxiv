#include "cxiv.h"
#include "env.h"
#include "eval.h"
#include "proc.h"

// Private functions

map* env_find(env* in, char* key);

env* cast_env(env*); // Needed because cons() returns a new obj of type PAIR.

map* env_find(env* in, char* key) {
    obj* walk = in;
    map* cur;

    if(in->type != ENV)
        die("Env find failed: outside of any scope.");

    while(walk != val_nil) {
        cur = car(walk)->map_value;

        if(map_contains(cur, key))
            return cur;

        walk = cdr(walk);
    }

    return NULL;
}

env* cast_env(env* in) {
    in->type = ENV;
    return in;
}

// Public functions

env* new_env() {
    return env_extend(val_nil, val_nil, val_nil);
}

void del_env(env* e) {
    del_list(e);
}

env* new_standard_env() {
    env* e = new_env();

    PUT_PROC("+", proc_add, e);
    PUT_PROC("-", proc_sub, e);
    PUT_PROC("*", proc_mul, e);
    PUT_PROC("/", proc_div, e);
    PUT_PROC("<", proc_lt, e);
    PUT_PROC(">", proc_gt, e);
    PUT_PROC("=", proc_eq, e);

    PUT_PROC("number?", proc_is_number, e);
    PUT_PROC("bool?", proc_is_bool, e);
    PUT_PROC("char?", proc_is_char, e);
    PUT_PROC("string?", proc_is_string, e);
    PUT_PROC("nil?", proc_is_nil, e);
    PUT_PROC("pair?", proc_is_pair, e);
    PUT_PROC("symbol?", proc_is_symbol, e);
    PUT_PROC("map?", proc_is_map, e);
    PUT_PROC("procedure?", proc_is_proc, e);
    PUT_PROC("eq?", proc_poly_eq, e);

    PUT_PROC("number->char", proc_num_to_char, e);
    PUT_PROC("char->number", proc_char_to_num, e);
    PUT_PROC("number->string", proc_num_to_string, e);
    PUT_PROC("string->number", proc_string_to_num, e);
    PUT_PROC("symbol->string", proc_symbol_to_string, e);
    PUT_PROC("string->symbol", proc_string_to_symbol, e);

    PUT_PROC("cons", proc_cons, e);
    PUT_PROC("car", proc_car, e);
    PUT_PROC("cdr", proc_cdr, e);
    PUT_PROC("set-car!", proc_set_car, e);
    PUT_PROC("set-cdr!", proc_set_cdr, e);
    PUT_PROC("list", proc_list, e);
    PUT_PROC("reverse", proc_reverse, e);

    PUT_PROC("split", proc_split, e);

    PUT_PROC("put", proc_put, e);
    PUT_PROC("get", proc_get, e);

    PUT_PROC("global-env", proc_global_env, e);
    PUT_PROC("env", proc_env, e);

    return e;
}

env* env_extend(obj* vars, obj* vals, env* in) {
    obj* m;

    if(list_len(vars) != list_len(vals))
        die("Cannot extend environment with unequal number of vars and vals.");

    // Create a new map, but change it to SYM type.
    m = new_omap();
    m->map_value->type = SYM;

    while(vars != val_nil) {
        if(car(vars)->type != SYMBOL)
            die("Cannot map value to non-symbol.");

        map_put(m->map_value, car(vars)->symbol_value, car(vals));

        vars = cdr(vars);
        vals = cdr(vals);
    }

    return cast_env(cons(m, in));
}

env* env_outer(env* in) {
    return cdr(in);
}

void env_define(env* in, char* key, obj* val) {

    // If nil, die.
    if(in->type != ENV)
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
