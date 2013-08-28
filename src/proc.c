#include "cxiv.h"
#include "proc.h"

obj* add_proc(obj* args) {
    double rv = 0;

    while(args != val_nil) {
        rv += car(args)->num_value;
        args = cdr(args);
    }

    return new_number(rv);
}

obj* sub_proc(obj* args) {
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

obj* mul_proc(obj* args) {
    double rv = 1;

    while(args != val_nil) {
        rv *= car(args)->num_value;
        args = cdr(args);
    }

    return new_number(rv);
}

obj* div_proc(obj* args) {
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
