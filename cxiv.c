#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    NUMBER,
    BOOL,
    CHAR,
    STRING
} obj_type;

typedef struct {
    obj_type type;
    union {
        double num_value;
        int bool_value;
        char char_value;
        char* string_value;
    };
} obj;

// Basic functions

void die(char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

// Predicates

int is_delim(int c) {
    return isspace(c) || c == EOF || c == '"' || c == ';';
}

// Object creation functions

obj* new_obj(obj_type type) {
    obj* o = malloc(sizeof(obj));
    o->type = type;

    if(!o)
        die("Out of memory.");

    return o;
}

obj* new_number(double input) {
    obj* o = new_obj(NUMBER);
    o->num_value = input;

    return o;
}

obj* new_boolean(int input) {
    obj* o = new_obj(BOOL);
    o->bool_value = input;

    return o;
}

obj* new_char(char input) {
    obj* o = new_obj(CHAR);
    o->char_value = input;

    return o;
}

obj* new_string(char* input) {
    obj* o = new_obj(STRING);
    char* value;

    size_t len = strlen(input);
    value = malloc(sizeof(char)*len);
    strncpy(value, input, len);

    o->string_value = value;

    return o;
}

void consume_whitespace(FILE* in) {
    int c;

    while((c = getc(in)) != EOF) {
        if(isspace(c))
            continue;

        if(c == ';') {
            while((c = getc(in)) != EOF && (c != '\n'));
            continue;
        }

        ungetc(c, in);
        break;
    }
}

int peek(FILE* in) {
    int c = getc(in);
    ungetc(c, in);
    return c;
}

obj* read(FILE* in) {
    int c;
    double num = 0;
    int sign = 1;

    consume_whitespace(in);

    c = getc(in);

    if(isdigit(c) || (c == '-' && isdigit(peek(in)))) {
        if(c == '-')
            sign = -1;
        else
            ungetc(c, in);

        while(isdigit(c = getc(in)))
            num = 10*num + (c - '0');

        num *= sign;

        if(!is_delim(c))
            die("Number has non-numeric component.");

        ungetc(c, in);
        return new_number(num);
    } else if(c == '#' && (peek(in) == 't' || peek(in) == 'f')) {
        if((c = getc(in)) == 't')
            return new_boolean(1);
        else
            return new_boolean(0);
    } else if(c == '#' && peek(in) == '\\') {
        // TODO: implement newline & space.
        getc(in);
        
        if(!isspace(c = getc(in))) {
            return new_char(c);
        } else {
            die("char prefix used with a weird value.");
        }
    /*} else if(c == '"') {
        while((c = getc(in)) != '"') {
            if(c == EOF)
                die("Reached EOF before \" character.");


        }
    */} else {
        die("Unidentifiable input.");
    }
}

obj* eval(obj* in) {
    return in;
}

void print(obj* in) {
    switch(in->type) {
    case NUMBER:
        printf("%f\n", in->num_value);
        break;
    case BOOL:
        printf("%s\n", in->bool_value ? "true" : "false");
        break;
    case CHAR:
        printf("#\\%c\n", in->char_value);
        break;
    case STRING:
        printf("%s\n", in->string_value);
        break;
    }
}

int main(int argc, char const *argv[]) {

    obj* intermed;

    while(1) {
        printf("? ");
        intermed = read(stdin);
        intermed = eval(intermed);
        print(intermed);
        printf("\n");
    }

    return 0;
}