#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "cxiv.h"
#include "obj.h"
#include "read.h"
#include "eval.h"
#include "print.h"
#include "proc.h"

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

    global_env = new_env();

    PUT_PROC("+", proc_add, global_env);
    PUT_PROC("-", proc_sub, global_env);
    PUT_PROC("*", proc_mul, global_env);
    PUT_PROC("/", proc_div, global_env);

    PUT_PROC("number?", proc_is_number, global_env);
    PUT_PROC("bool?", proc_is_bool, global_env);
    PUT_PROC("char?", proc_is_char, global_env);
    PUT_PROC("string?", proc_is_string, global_env);
    PUT_PROC("nil?", proc_is_nil, global_env);
    PUT_PROC("pair?", proc_is_pair, global_env);
    PUT_PROC("symbol?", proc_is_symbol, global_env);
    PUT_PROC("map?", proc_is_map, global_env);
    PUT_PROC("procedure?", proc_is_proc, global_env);
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
