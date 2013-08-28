#include "cxiv.h"
#include "print.h"
#include "hash.h"

void print_list(obj* in) {
    print(car(in));

    // if cdr is a pair, print this as a list.
    switch(cdr(in)->type) {
    case PAIR:
        printf(" ");
        print_list(cdr(in));
        break;
    case NIL:
        break;
    default:
        printf(" . ");
        print(cdr(in));
    }
}

void print_map(obj* in) {
    map_iter i = map_start();
    cell* cur;
    cell* next = map_next(in->map_value, i);

    if(!next)
        return;

    while(true) {
        cur = next;
        next = map_next(in->map_value, i);

        print(cur->key);
        printf(": ");
        print(cur->val);

        if(next)
            printf(" ");
        else
            break;
    }
}

void print(obj* in) {
    switch(in->type) {
    case NUMBER:
        printf("%g", in->num_value);
        break;
    case BOOL:
        printf("%s", in->bool_value ? "true" : "false");
        break;
    case CHAR:
        switch(in->char_value) {
        case ' ':
            printf("#\\space");
            break;
        case '\n':
            printf("#\\newline");
            break;
        default:
            printf("#\\%c", in->char_value);
        }
        break;
    case STRING:
        printf("\"%s\"", in->string_value);
        break;
    case SYMBOL:
        printf("%s", in->symbol_value);
        break;
    case NIL:
        printf("nil");
        break;
    case PAIR:
        printf("(");
        print_list(in);
        printf(")");
        break;
    case MAP:
        printf("{");
        print_map(in);
        printf("}");
        break;
    case PROC_NATIVE:
        printf("#<native proc %s>", in->proc_native.name);
        break;
    }
}
