#include "util.h"

void die(char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}
