#ifndef CXIV_H
#define CXIV_H

#include "obj.h"
#include "hash.h"
#include "env.h"

// globals

#define true 1
#define false 0

obj* val_false;
obj* val_true;
obj* val_nil;

symmap* sym_cache;

obj* sym_quote;
obj* sym_define;
obj* sym_set;
obj* sym_if;
obj* sym_and;
obj* sym_or;
obj* sym_lambda;

env* global_env;

#endif
