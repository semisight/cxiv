#include "cxiv.h"
#include "obj.h"

void make_globals() {
    val_false = new_obj(BOOL);
    val_true = new_obj(BOOL);
    val_nil = new_obj(NIL);

    val_false->bool_value = 0;
    val_true->bool_value = 1;

    sym_tbl = new_map();
}

obj* new_obj(obj_type type) {
    obj* o = malloc(sizeof(obj));
    o->type = type;

    if(!o)
        die("Out of memory.");

    return o;
}

obj* new_number(double input) {
    obj* o = new_obj(NUMBER);
    o->num_value = input;

    return o;
}

obj* new_boolean(int input) {
    return input ? val_true : val_false;
}

obj* new_char(char input) {
    obj* o = new_obj(CHAR);
    o->char_value = input;

    return o;
}

// The new object will *own* the input string. If you plan to free it then
// copy it first.
obj* new_string(char* input) {
    obj* o = new_obj(STRING);
    o->string_value = input;

    return o;
}

// Like new_string(), this will own the input string.
obj* new_symbol(char* input) {
    obj* o;

    if(!(o = map_get(sym_tbl, input))) {
        o = new_obj(SYMBOL);
        o->symbol_value = input;

        map_put(sym_tbl, input, o);
    }

    return o;
}

obj* cons(obj* first, obj* second) {
    obj* o = new_obj(PAIR);

    o->pair.car = first;
    o->pair.cdr = second;

    return o;
}

obj* car(obj* in) {
    if(in->type != PAIR)
        die("Cannot get car of non-pair.");

    return in->pair.car;
}

obj* cdr(obj* in) {
    if(in->type == PAIR)
        return in->pair.cdr;
    if(in->type == NIL)
        return val_nil;

    die("Cannot get cdr of non-pair.");
    return 0; // For clang.
}
