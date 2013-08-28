#ifndef PROC_H
#define PROC_H

#include "obj.h"

#define PUT_PROC(n, p, e) _define_var(n, new_proc(n, p), e)

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

// Integer ops

obj* proc_add(obj*);

obj* proc_sub(obj*);

obj* proc_mul(obj*);

obj* proc_div(obj*);

#endif