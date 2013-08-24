#include "print.h"

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

void print(obj* in) {
    switch(in->type) {
    case NUMBER:
        printf("%f", in->num_value);
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
    }
}
