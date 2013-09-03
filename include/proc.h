#ifndef PROC_H
#define PROC_H

#include "obj.h"

#define PUT_PROC(n, p, e) _define_var(new_symbol(n), new_proc(n, p), e)

// General predicates

obj* proc_is_number(obj*);

obj* proc_is_bool(obj*);

obj* proc_is_char(obj*);

obj* proc_is_string(obj*);

obj* proc_is_nil(obj*);

obj* proc_is_pair(obj*);

obj* proc_is_symbol(obj*);

obj* proc_is_map(obj*);

obj* proc_is_proc(obj*);

obj* proc_poly_eq(obj*);

// Type conversions

obj* proc_num_to_char(obj*);

obj* proc_char_to_num(obj*);

obj* proc_num_to_string(obj*);

obj* proc_string_to_num(obj*);

obj* proc_symbol_to_string(obj*);

obj* proc_string_to_symbol(obj*);

// Integer ops

obj* proc_add(obj*);

obj* proc_sub(obj*);

obj* proc_mul(obj*);

obj* proc_div(obj*);

obj* proc_lt(obj*);

obj* proc_gt(obj*);

obj* proc_eq(obj*);

// List ops

obj* proc_cons(obj*);

obj* proc_car(obj*);

obj* proc_cdr(obj*);

obj* proc_set_car(obj*);

obj* proc_set_cdr(obj*);

obj* proc_list(obj*);

#endif