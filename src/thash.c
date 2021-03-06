#include <stdlib.h>
#include <stdio.h>

#include "hash.h"
#include "obj.h"

void check_map(symmap* m, char* key) {
    printf("Getting \"%s\"\n", key);

    obj* o = map_get(m, key);

    if(o)
        printf("%s: %f\n", key, o->num_value);
    else
        printf("Key %s not set in map.\n", key);
}

int main(int argc, char const *argv[]) {
    
    symmap* m = new_map(SYM);

    map_put(m, "hello", new_number(2.0));
    map_put(m, "world", new_number(3.0));
    map_put(m, "haiku", new_number(4.5));

    map_iter i = map_start();
    cell* cur;
    while((cur = map_next(m, i))) {
        printf("%s: %f\n", cur->key, cur->val->num_value);
    }

    return 0;
}