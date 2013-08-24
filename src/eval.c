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
        break;
    default: // NIL and SYMBOL
        die("Cannot evaluate symbols or nil directly. Remember to quote.");
        return NULL; // For clang.
    }
}
