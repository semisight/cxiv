#ifndef EVAL_H
#define EVAL_H

#include "obj.h"

/*
 * eval.h
 *
 * Handles the evaluation of the object tree after it is parsed.
 */

obj* get_verb(obj*);

obj* define_var(obj*, env*);

obj* set_var(obj*, env*);

obj* eval_if(obj*, env*);

obj* eval_map(obj*, env*);

obj* eval_symbol(obj*, env*);

obj* eval_list(obj*, env*);

obj* eval(obj*, env*);

#endif
