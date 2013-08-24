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

obj* eval_list(obj* in);

obj* eval(obj* in);

#endif
