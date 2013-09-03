#include "cxiv.h"
#include "eval.h"

obj* get_verb(obj* in) {
    return car(in);
}

void _define_var(obj* var, obj* val, env* e) {
    env_define(e, var->symbol_value, val);
}

void _set_var(obj* var, obj* val, env* e) {
    env_set(e, var->symbol_value, val);
}

obj* define_var(obj* in, env* e) {
    obj* var = NULL;
    obj* val = NULL;
    obj* args;
    obj* body;

    if(car(in)->type == SYMBOL) {
        var = car(in);
        val = eval(cadr(in), e);
    } else if(car(in)->type == PAIR) {
        var = caar(in);
        args = cdar(in);
        body = cdr(in);

        val = new_compound_proc(var->symbol_value, args, body, e);
    }

    _define_var(var, val, e);
    return var;
}

obj* set_var(obj* in, env* e) {
    if(in == val_nil || list_len(in) != 2)
        die("set! takes 2 arguments.");

    obj* var = car(in);
    obj* val = eval(cadr(in), e);

    if(var->type != SYMBOL)
        die("Only symbols can be defined.");

    _set_var(var, val, e);
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

obj* eval_and(obj* in, env* e) {
    obj* arg;

    if(in == val_nil)
        return val_true;

    while(in != val_nil) {
        arg = eval(car(in), e);
        if(arg == val_false || arg == val_nil)
            return arg;

        in = cdr(in);
    }

    return arg;
}

obj* eval_or(obj* in, env* e) {
    obj* arg;

    if(in == val_nil)
        return val_false;

    while(in != val_nil) {
        arg = eval(car(in), e);
        if(arg != val_false && arg != val_nil)
            return arg;

        in = cdr(in);
    }

    return arg;
}

obj* eval_lines(obj* in, env* e) {
    obj* ret;

    do {
        ret = eval(car(in), e);
    } while((in = cdr(in)) != val_nil);

    return ret;
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
    } else if(verb == sym_and) {
        return eval_and(rest, e);
    } else if(verb == sym_or) {
        return eval_or(rest, e);
    } else if(verb == sym_lambda) {
        obj* args = car(rest);
        obj* body = cdr(rest);

        return new_compound_proc(NULL, args, body, e);
    } else if(verb == sym_do) {
        return eval_lines(rest, e);
    } else {
        obj* p = eval(verb, e);
        obj* args = eval_arguments(rest, e);

        if(p->type == PROC_NATIVE) {
            return p->proc_native.call(args);
        } else if(p->type == PROC_COMPOUND) {
            env* ext = env_extend(p->proc_compound.arg_list,
                                  args,
                                  p->proc_compound.env);

            return eval_lines(p->proc_compound.body, ext);
        } else {
            die("Cannot execute non-proc.");
            return 0;
        }
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
    case PROC_COMPOUND:
    case NIL:
        die("Cannot evaluate procs or nil directly. Remember to quote/apply.");
        return NULL; // For clang.
    }
}
