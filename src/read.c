#include "read.h"

// Predicates

int is_delim(int c) {
    return isspace(c) || c == EOF || c == '"' || c == ';' || c == ')';
}

// Stream processors

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

void consume_expected(FILE* in, const char* expect) {
    int len = strlen(expect);
    char* actual = malloc(sizeof(char) * len);

    if(fgets(actual, len+1, in) == NULL) {
        die("Read error.");
    }

    if(feof(in) || strcmp(expect, actual) != 0) {
        die("Expected different string.");
    }
}

int peek(FILE* in) {
    int c = getc(in);
    ungetc(c, in);
    return c;
}

double read_num(char c, FILE* in) {
    double num = 0;
    int sign = 1;

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
    return num;
}

int read_bool(char c, FILE* in) {
    if((c = getc(in)) == 't')
        return 1;
    else
        return 0;
}

char read_char(char c, FILE* in) {
    // Clear slash character.
    getc(in);
    
    if(isspace(c = getc(in)))
        die("char prefix used with a weird value.");

    if(c == 's' && peek(in) == 'p') {
        consume_expected(in, "pace");
        return ' ';
    }

    if(c == 'n' && peek(in) == 'e') {
        consume_expected(in, "ewline");
        return '\n';
    }

    return c;
}

char* read_string(char c, FILE* in) {
    char* buf = malloc(sizeof(char)*BUFFER_INIT);
    unsigned int cur = 0, max = BUFFER_INIT;

    while((c = getc(in)) != '"') {
        if(c == EOF)
            die("Reached EOF before \" character.");

        if(c == '\\') {
            switch(c = getc(in)) {
            case 'n':
                buf[cur++] = '\n';
                break;
            case '"':
                buf[cur++] = '"';
                break;
            case '\\':
                buf[cur++] = '\\';
                break;
            default:
                die("Unexpected escape character.");
            }
        } else {
            buf[cur++] = c;
        }

        // Check if we've hit our limit
        if(cur == max) {
            // If so, double our capacity
            max *= 2;
            buf = realloc(buf, sizeof(char) * max);
        }
    }

    return buf;
}

obj* read_pair(FILE* in) {

    consume_whitespace(in);

    if(peek(in) == ')') {
        getc(in);
        return val_nil;
    }

    // otherwise, must be a pair.
    obj* first = read(in);

    consume_whitespace(in);

    // Improper pair
    if(peek(in) == '.') {
        // Get rid of dot.
        getc(in);

        obj* second = read(in);

        consume_whitespace(in);

        if(peek(in) != ')')
            die("Expected a close paren.");

        getc(in);

        return cons(first, second);
    } else {
        return cons(first, read_pair(in));
    }
}

obj* read(FILE* in) {
    int c;

    consume_whitespace(in);

    c = getc(in);

    if(isdigit(c) || (c == '-' && isdigit(peek(in)))) {
        return new_number(read_num(c, in));
    } else if(c == '#' && (peek(in) == 't' || peek(in) == 'f')) {
        return new_boolean(read_bool(c, in));
    } else if(c == '#' && peek(in) == '\\') {
        return new_char(read_char(c, in));
    } else if(c == '"') {
        return new_string(read_string(c, in));
    } else if(c == '(') {
        return read_pair(in);
    } else if(c == EOF) {
        printf("Thanks!\n");
        exit(1);
    } else {
        die("Unidentifiable input.");
        return 0; // to make clang happy.
    }
}
