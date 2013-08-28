#include "cxiv.h"
#include "eval.h"

obj* get_verb(obj* in) {
    if(in->type != PAIR || car(in)->type != SYMBOL)
        die("Cannot get verb for non-pair.");

    return car(in);
}

void _define_var(char* var, obj* val, env* e) {
    env_define(e, var, val);
}

void _set_var(char* var, obj* val, env* e) {
    env_set(e, var, val);
}

obj* define_var(obj* in, env* e) {
    if(in == val_nil || list_len(in) != 2)
        die("Define not given 2 arguments.");

    obj* var = car(in);
    obj* val = eval(cadr(in), e);

    if(var->type != SYMBOL)
        die("Only symbols can be defined.");

    _define_var(var->symbol_value, val, e);
    return var;
}

obj* set_var(obj* in, env* e) {
    if(in == val_nil || list_len(in) != 2)
        die("set! takes 2 arguments.");

    obj* var = car(in);
    obj* val = eval(cadr(in), e);

    if(var->type != SYMBOL)
        die("Only symbols can be defined.");

    _set_var(var->symbol_value, val, e);
    return val;
}

// Eval routines

obj* eval_if(obj* in, env* e) {
    if(in == val_nil || list_len(in) != 3)
        die("if takes 3 arguments.");

    obj* pred = eval(car(in), e);
    obj* cnsq = cadr(in);
    obj* alt = caddr(in);

    if(pred != val_nil && pred != val_false)
        return cnsq;
    else
        return alt;
}

obj* eval_map(obj* in, env* e) {
    map_iter i = map_start();
    cell* cur;

    while((cur = map_next(in->map_value, i))) {
        cur->key = eval(cur->key, e);
        cur->val = eval(cur->val, e);
    }

    return in;
}

obj* eval_symbol(obj* in, env* e) {
    return env_get(e, in->symbol_value);
}

obj* eval_arguments(obj* in, env* e) {
    if(in == val_nil)
        return val_nil;

    return cons(eval(car(in), e), eval_arguments(cdr(in), e));
}

obj* eval_list(obj* in, env* e) {
    obj* verb = get_verb(in);
    obj* rest = cdr(in);

    if(verb == sym_quote) {
        return cadr(in);
    } else if(verb == sym_define) {
        return define_var(rest, e);
    } else if(verb == sym_set) {
        return set_var(rest, e);
    } else if(verb == sym_if) {
        return eval(eval_if(rest, e), e);
    } else {
        obj* p = eval(verb, e);
        obj* args = eval_arguments(rest, e);

        return p->proc_native.call(args);
    }
}

obj* eval(obj* in, env* e) {
    // See what kind of object we have, then evaluate if necessary.
    switch(in->type) {
    case NUMBER:
    case BOOL:
    case CHAR:
    case STRING:
        return in;
    case PAIR:
        return eval_list(in, e);
    case MAP:
        return eval_map(in, e);
    case SYMBOL:
        return eval_symbol(in, e);
    case PROC_NATIVE:
    case NIL:
        die("Cannot evaluate procs or nil directly. Remember to quote/apply.");
        return NULL; // For clang.
    }
}
