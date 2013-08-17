#include "obj.h"

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
    obj* o = new_obj(BOOL);
    o->bool_value = input;

    return o;
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
