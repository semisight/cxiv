#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "obj.h"
#include "read.h"
#include "eval.h"
#include "print.h"

// Basic functions

int main(int argc, char const *argv[]) {

    // setup
    make_globals();

    obj* intermed;

    while(1) {
        printf("? ");
        intermed = read(stdin);
        intermed = eval(intermed);
        print(intermed);
        printf("\n\n");
    }

    return 0;
}
