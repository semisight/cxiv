#include "read.h"

int is_delim(int c) {
    return isspace(c) || c == EOF || c == '"' || c == ';';
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
