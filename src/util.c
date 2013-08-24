#include "util.h"

void die(char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

char* fulcpy(char* in) {
    size_t len = strlen(in) + 1;

    char* out = malloc(sizeof(char) * len);
    if(!out)
        die("Out of memory.");

    strlcpy(out, in, len);
    return out;
}
