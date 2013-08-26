#ifndef EVAL_H
#define EVAL_H

#include "obj.h"

/*
 * eval.h
 *
 * Handles the evaluation of the object tree after it is parsed.
 */

// Predicates

int is_quoted(obj*);

// Eval routines

obj* eval_list(obj*);

obj* eval_map(obj*);

obj* eval(obj*);

#endif
