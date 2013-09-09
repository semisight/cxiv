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
    sym_if = new_symbol("if");
    sym_and = new_symbol("and");
    sym_or = new_symbol("or");
    sym_lambda = new_symbol("lambda");
    sym_do = new_symbol("do");
    sym_apply = new_symbol("apply");
    sym_eval= new_symbol("eval");

    global_env = new_standard_env();
}

int main(int argc, char const *argv[]) {

    // setup
    make_globals();

    obj* intermed;

    while(1) {
        printf("? ");
        intermed = read(stdin);
        intermed = eval(intermed, global_env);
        print(intermed);
        printf("\n\n");
    }

    return 0;
}
