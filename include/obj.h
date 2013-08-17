#ifndef OBJ_H
#define OBJ_H

#include <stdlib.h>
#include <string.h>

#include "util.h"

/*
 * obj.h
 *
 * Object definition, creation and modification functions.
 */

typedef enum {
    NUMBER,
    BOOL,
    CHAR,
    STRING
} obj_type;

typedef struct {
    obj_type type;
    union {
        double num_value;
        int bool_value;
        char char_value;
        char* string_value;
    };
} obj;

obj* new_obj(obj_type type);

obj* new_number(double input);

obj* new_boolean(int input);

obj* new_char(char input);

obj* new_string(char* input);

#endif
