#include "cxiv.h"
#include "read.h"

// Predicates

int is_in(int c, const char* list) {
    for(size_t i=0; i<strlen(list); i++)
        if((char)c == list[i])
            return true;

    return false;
}

int is_delim(int c) {
    return isspace(c) || c == EOF || c == ';' || c == ')';
}

int is_initial(int c) {
    return isalpha(c) || is_in(c, "!$%&*/:<=>?^_~");
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

    if(is_in(c, "+-")) {
        if(c == '-')
            sign = -1;
    } else {
        ungetc(c, in);
    }

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
        return true;
    else
        return false;
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
    char* buf = malloc(sizeof(char) * BUFFER_INIT);
    unsigned int cur = 0, max = BUFFER_INIT;

    while((c = getc(in)) != '"') {
        if(c == EOF)
            die("Reached EOF before \" character.");

        // Check if we've hit our limit (save room for null-terminator)
        if(cur == max-1) {
            // If so, double our capacity
            max *= 2;
            buf = realloc(buf, sizeof(char) * max);
        }

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
    }

    buf[cur] = '\0';

    return buf;
}

char* read_symbol(char c, FILE* in) {
    if(is_in(c, "+-")) {
        if(!isspace(peek(in)) && !is_delim(peek(in)))
            die("+ or - are single letter symbols.");
        
        return (c == '+') ? "+" : "-";
    }

    ungetc(c, in);

    char* buf = malloc(sizeof(char) * BUFFER_INIT);
    unsigned int cur = 0, max = BUFFER_INIT;

    while((c = getc(in)) != EOF) {
        // Check if we're done reading.
        if(!isdigit(c) && !is_initial(c) && !is_in(c, "+-.@")) {
            ungetc(c, in);
            break;
        }

        // Check if we've hit our limit (save room for null-terminator)
        if(cur == max-1) {
            // If so, double our capacity
            max *= 2;
            buf = realloc(buf, sizeof(char) * max);
        }

        // Otherwise copy in.
        buf[cur++] = c;
    }

    buf[cur] = '\0';

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

    if(isdigit(c) || (is_in(c, "+-") && isdigit(peek(in)))) {   // number
        return new_number(read_num(c, in));
    } else if(c == '#' && is_in(peek(in), "tf")) {              // boolean
        return new_boolean(read_bool(c, in));
    } else if(c == '#' && peek(in) == '\\') {                   // char
        return new_char(read_char(c, in));
    } else if(c == '"') {                                       // string
        char* str = read_string(c, in);
        obj* o = new_string(str);
        free(str);
        return o;
    } else if(c == '(') {                                       // list/nil
        return read_pair(in);
    } else if(is_initial(c) || is_in(c, "+-")) {                // symbol
        char* str = read_symbol(c, in);
        obj* o = new_symbol(str);
        free(str);
        return o;
    } else if(c == '\'') {                                      // quoted sexp
        return cons(sym_quote, cons(read(in), val_nil));
    } else if(c == EOF) {
        printf("Thanks!\n");
        exit(1);
    } else {
        die("Unidentifiable input.");
        return NULL; // to make clang happy.
    }
}
