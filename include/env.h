#ifndef ENV_H
#define ENV_H

#include "obj.h"

env* new_env();

env* new_standard_env();

env* env_extend(obj*, obj*, env*);

void env_define(env*, char*, obj*);

obj* env_get(env*, char*);

void env_set(env*, char*, obj*);

#endif
