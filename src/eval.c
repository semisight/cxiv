#include "cxiv.h"
#include "eval.h"

// Predicates

int is_quoted(obj* in) {
    return in->type == PAIR && car(in) == sym_quote;
}

// Eval routines

obj* eval_list(obj* in) {
    if(is_quoted(in)) {
        return cadr(in);
    } else {
        die("Cannot eval non-quoted lists yet.");
        return NULL; // For clang.
    }
}

obj* eval_map(obj* in) {
    map_iter i = map_start();
    cell* cur;

    while((cur = map_next(in->map_value, i))) {
        cur->key = eval(cur->key);
        cur->val = eval(cur->val);
    }

    return in;
}

obj* eval(obj* in) {
    // See what kind of object we have, then evaluate if necessary.
    switch(in->type) {
    case NUMBER:
    case BOOL:
    case CHAR:
    case STRING:
        return in;
    case PAIR:
        return eval_list(in);
    case MAP:
        return eval_map(in);
    case NIL:
    case SYMBOL:
        die("Cannot evaluate symbols or nil directly. Remember to quote.");
        return NULL; // For clang.
    }
}
