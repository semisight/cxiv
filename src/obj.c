#include "obj.h"

void make_globals() {
    val_false = new_obj(BOOL);
    val_true = new_obj(BOOL);
    val_nil = new_obj(NIL);

    val_false->bool_value = 0;
    val_true->bool_value = 1;
    val_nil->pair.car = NULL;
    val_nil->pair.cdr = val_nil;
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

obj* new_string(char* input) {
    obj* o = new_obj(STRING);
    char* value;

    size_t len = strlen(input);
    value = malloc(sizeof(char)*len);
    strncpy(value, input, len);

    o->string_value = value;

    return o;
}

obj* cons(obj* first, obj* second) {
    obj* o = new_obj(PAIR);

    o->pair.car = first;
    o->pair.cdr = second;

    return o;
}

obj* car(obj* in) {
    return in->pair.car;
}

obj* cdr(obj* in) {
    return in->pair.cdr;
}
