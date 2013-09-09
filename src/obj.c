#include "cxiv.h"
#include "obj.h"

obj* new_obj(obj_type type) {
    obj* o = malloc(sizeof(obj));
    o->type = type;

    if(!o)
        die("Out of memory.");

    return o;
}

obj* clone_obj(obj* in) {
    obj* rv;

    switch(in->type) {
    case NUMBER:
        return new_number(in->num_value);
    case BOOL:
    case SYMBOL:
    case NIL:
    case PROC_NATIVE:
    case PROC_COMPOUND:
        return in;
    case CHAR:
        return new_char(in->char_value);
    case STRING:
        return new_string(in->string_value);
    case ENV:
    case PAIR:
        return cons(clone_obj(car(in)), clone_obj(cdr(in)));
    case MAP:
        rv = new_obj(MAP);
        rv->map_value = map_clone(in->map_value);

        return rv;
    }
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

// Will *not* own the input. Req'd because it may accept const char*s which it
// should not free.
obj* new_string(char* input) {
    obj* o = new_obj(STRING);
    o->string_value = fulcpy(input);

    return o;
}

// Will *not* own the input. Req'd because it may accept const char*s which it
// should not free.
obj* new_symbol(char* input) {
    obj* o;

    if(!(o = map_get(sym_cache, input))) {
        o = new_obj(SYMBOL);
        o->symbol_value = fulcpy(input);

        map_put(sym_cache, input, o);
    }

    return o;
}

obj* new_omap() {
    obj* o = new_obj(MAP);
    o->map_value = new_map(OBJ);

    return o;
}

obj* new_proc(char* name, proc input) {
    obj* o = new_obj(PROC_NATIVE);
    o->proc_native.name = fulcpy(name);
    o->proc_native.call = input;

    return o;
}

obj* new_compound_proc(char* name, obj* args, obj* body, env* e) {
    obj* o = new_obj(PROC_COMPOUND);

    if(name)
        o->proc_compound.name = name;
    else
        o->proc_compound.name = "lambda";

    o->proc_compound.arg_list = args;
    o->proc_compound.body = body;
    o->proc_compound.env = e;

    return o;
}

obj* cons(obj* first, obj* second) {
    obj* o = new_obj(PAIR);

    o->pair.car = first;
    o->pair.cdr = second;

    return o;
}

obj* car(obj* in) {
    if(in->type != PAIR && in->type != ENV)
        die("Cannot get car of non-pair.");

    return in->pair.car;
}

obj* cdr(obj* in) {
    if(in->type == PAIR || in->type == ENV)
        return in->pair.cdr;
    if(in->type == NIL)
        return val_nil;

    die("Cannot get cdr of non-pair.");
    return false; // For clang.
}

int list_len(obj* list) {
    if(list->type != PAIR && list->type != NIL)
        die("Cannot get length of non-list.");

    if(list == val_nil)
        return 0;

    int i = 1;
    while((list = cdr(list)) != val_nil)
        i++;

    return i;
}

int is_equal(obj* a, obj* b) {
    // Quick check for types.
    if(a->type != b->type)
        return false;

    switch(a->type) {
    case NUMBER:
        return a->num_value == b->num_value;
    case BOOL:
    case SYMBOL:
    case PROC_NATIVE:
    case PROC_COMPOUND:
        return a == b;
    case CHAR:
        return a->char_value == b->char_value;
    case STRING:
        return strcmp(a->string_value, b->string_value) == 0;
    case NIL:
        return true; // Must both be nil
    case PAIR:
    case ENV:
        return is_equal(car(a), car(b)) && is_equal(cdr(a), cdr(b));
    case MAP:
        if(a->map_value->size != b->map_value->size) // If sizes are equal...
            return false;

        // We have to check to see if everything in a is also in b
        map_iter i = map_start();
        cell* cur;

        while((cur = map_next(a->map_value, i))) {
            if(map_get(b->map_value, cur->key) != cur->val)
                return false;
        }

        return true;
    }
}
