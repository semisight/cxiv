#include "print.h"

void print(obj* in) {
    switch(in->type) {
    case NUMBER:
        printf("%f\n", in->num_value);
        break;
    case BOOL:
        printf("%s\n", in->bool_value ? "true" : "false");
        break;
    case CHAR:
        switch(in->char_value) {
        case ' ':
            printf("#\\space\n");
            break;
        case '\n':
            printf("#\\newline\n");
            break;
        default:
            printf("#\\%c\n", in->char_value);
        }
        break;
    case STRING:
        printf("%s\n", in->string_value);
        break;
    }
}
