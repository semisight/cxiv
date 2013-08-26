#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "cxiv.h"
#include "obj.h"
#include "read.h"
#include "eval.h"
#include "print.h"

// Basic functions

void make_globals() {
    val_false = new_obj(BOOL);
    val_true = new_obj(BOOL);
    val_nil = new_obj(NIL);

    val_false->bool_value = false;
    val_true->bool_value = true;

    sym_cache = new_map(SYM);

    sym_quote = new_symbol("quote");
    sym_define = new_symbol("define");
    sym_set = new_symbol("set!");
}

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
