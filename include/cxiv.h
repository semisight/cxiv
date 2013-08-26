#ifndef CXIV_H
#define CXIV_H

#include "obj.h"
#include "hash.h"

// globals

#define true 1
#define false 0

obj* val_false;
obj* val_true;
obj* val_nil;

symmap* sym_cache;

obj* sym_quote;

#endif