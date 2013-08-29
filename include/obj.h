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
    STRING,
    NIL,
    PAIR,
    SYMBOL,
    MAP,
    PROC_NATIVE,
    PROC_COMPOUND
} obj_type;

typedef struct obj_t* (*proc)(struct obj_t*);

struct map_t;

typedef struct obj_t {
    obj_type type;
    union {
        double num_value;
        int bool_value;
        char char_value;
        char* string_value;
        char* symbol_value;
        struct map_t* map_value;
        struct {
            struct obj_t* car;
            struct obj_t* cdr;
        } pair;
        struct {
            char* name;
            proc call;
        } proc_native;
        struct {
            char* name;
            obj* arg_list;
            obj* body;
            env* env;
        } proc_compound;
    };
} obj;

obj* new_obj(obj_type type);

obj* new_number(double input);

obj* new_boolean(int input);

obj* new_char(char input);

obj* new_string(char* input);

obj* new_omap();

obj* new_proc(char*, proc);

obj* cons(obj*, obj*);

obj* car(obj*);

obj* cdr(obj*);

int list_len(obj*);

#define caar(obj) car(car(obj))
#define cadr(obj) car(cdr(obj))
#define cdar(obj) cdr(car(obj))
#define cddr(obj) cdr(cdr(obj))
#define caaar(obj) car(car(car(obj)))
#define caadr(obj) car(car(cdr(obj)))
#define cadar(obj) car(cdr(car(obj)))
#define caddr(obj) car(cdr(cdr(obj)))
#define cdaar(obj) cdr(car(car(obj)))
#define cdadr(obj) cdr(car(cdr(obj)))
#define cddar(obj) cdr(cdr(car(obj)))
#define cdddr(obj) cdr(cdr(cdr(obj)))

obj* new_symbol(char* input);

int is_equal(obj*, obj*);

#endif
