#include "cxiv.h"
#include "proc.h"

// General predicates

obj* proc_is_number(obj* in) {
    return car(in)->type == NUMBER ? val_true : val_false;
}

obj* proc_is_bool(obj* in) {
    return car(in)->type == BOOL ? val_true : val_false;
}

obj* proc_is_char(obj* in) {
    return car(in)->type == CHAR ? val_true : val_false;
}

obj* proc_is_string(obj* in) {
    return car(in)->type == STRING ? val_true : val_false;
}

obj* proc_is_nil(obj* in) {
    return car(in)->type == NIL ? val_true : val_false;
}

obj* proc_is_pair(obj* in) {
    return car(in)->type == PAIR ? val_true : val_false;
}

obj* proc_is_symbol(obj* in) {
    return car(in)->type == SYMBOL ? val_true : val_false;
}

obj* proc_is_map(obj* in) {
    return car(in)->type == MAP ? val_true : val_false;
}

obj* proc_is_proc(obj* in) {
    return car(in)->type == PROC_NATIVE ? val_true : val_false;
}

// Integer ops

obj* proc_add(obj* args) {
    double rv = 0;

    while(args != val_nil) {
        rv += car(args)->num_value;
        args = cdr(args);
    }

    return new_number(rv);
}

obj* proc_sub(obj* args) {
    double rv = 0;

    if(args == val_nil)
        die("Expected at least 1 argument.");

    if(list_len(args) == 1)
        return new_number(-1 * car(args)->num_value);

    rv = car(args)->num_value;
    args = cdr(args);

    while(args != val_nil) {
        rv -= car(args)->num_value;
        args = cdr(args);
    }

    return new_number(rv);
}

obj* proc_mul(obj* args) {
    double rv = 1;

    while(args != val_nil) {
        rv *= car(args)->num_value;
        args = cdr(args);
    }

    return new_number(rv);
}

obj* proc_div(obj* args) {
    double rv = 1;

    if(args == val_nil)
        die("Expected at least 1 argument.");

    if(list_len(args) == 1)
        return new_number(1.0 / car(args)->num_value);

    rv = car(args)->num_value;
    args = cdr(args);

    while(args != val_nil) {
        rv /= car(args)->num_value;
        args = cdr(args);
    }

    return new_number(rv);
}
