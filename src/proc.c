#include "cxiv.h"
#include "proc.h"
#include "read.h"
#include "eval.h"

// General predicates

obj* proc_is_number(obj* in) {
    return car(in)->type == NUMBER ? val_true : val_false;
}

obj* proc_is_bool(obj* in) {
    return car(in)->type == BOOL ? val_true : val_false;
}

obj* proc_is_char(obj* in) {
    return car(in)->type == CHAR ? val_true : val_false;
}

obj* proc_is_string(obj* in) {
    return car(in)->type == STRING ? val_true : val_false;
}

obj* proc_is_nil(obj* in) {
    return car(in)->type == NIL ? val_true : val_false;
}

obj* proc_is_pair(obj* in) {
    return car(in)->type == PAIR ? val_true : val_false;
}

obj* proc_is_symbol(obj* in) {
    return car(in)->type == SYMBOL ? val_true : val_false;
}

obj* proc_is_map(obj* in) {
    return car(in)->type == MAP ? val_true : val_false;
}

obj* proc_is_proc(obj* in) {
    return car(in)->type == PROC_NATIVE ? val_true : val_false;
}

obj* proc_poly_eq(obj* in) {
    return is_equal(car(in), cadr(in)) ? val_true : val_false;
}

// Type conversions

obj* proc_num_to_char(obj* in) {
    int val = (int)round(car(in)->num_value);

    if(val < 0 || val > 255)
        die("Number must be between 0-255 inclusive for conversion to char.");

    return new_char((char)val);
}

obj* proc_char_to_num(obj* in) {
    return new_number((double)car(in)->char_value);
}

obj* proc_num_to_string(obj* in) {
    char* buf[20]; // 20 Characters should be enough for anyone...

    snprintf((char*)buf, 20, "%g", car(in)->num_value);
    return new_string((char*)buf);
}

obj* proc_string_to_num(obj* in) {
    char* str = car(in)->string_value;
    double num = 0;
    int sign = 1, digits_after_dec = -1;

    if(is_in(*str, "+-")) {
        if(*str == '-')
            sign = -1;
        str++;
    }

    while(isdigit(*str) || *str == '.') {
        if(isdigit(*str)) {
            num = 10*num + (*str - '0');

            if(digits_after_dec != -1)
                digits_after_dec++;
        } else if(*str == '.') {
            if(digits_after_dec != -1)
                die("Cannot have 2 decimal points in a number.");

            digits_after_dec = 0;
        }

        str++;
    }

    num *= sign;

    if(digits_after_dec != -1)
        num /= pow(10, digits_after_dec);

    return new_number(num);
}

obj* proc_symbol_to_string(obj* in) {
    return new_string(car(in)->symbol_value);
}

obj* proc_string_to_symbol(obj* in) {
    return new_symbol(car(in)->string_value);
}

// Integer ops

obj* proc_add(obj* args) {
    double rv = 0;

    while(args != val_nil) {
        rv += car(args)->num_value;
        args = cdr(args);
    }

    return new_number(rv);
}

obj* proc_sub(obj* args) {
    double rv = 0;

    if(args == val_nil)
        die("Expected at least 1 argument.");

    if(list_len(args) == 1)
        return new_number(-1 * car(args)->num_value);

    rv = car(args)->num_value;
    args = cdr(args);

    while(args != val_nil) {
        rv -= car(args)->num_value;
        args = cdr(args);
    }

    return new_number(rv);
}

obj* proc_mul(obj* args) {
    double rv = 1;

    while(args != val_nil) {
        rv *= car(args)->num_value;
        args = cdr(args);
    }

    return new_number(rv);
}

obj* proc_div(obj* args) {
    double rv = 1;

    if(args == val_nil)
        die("Expected at least 1 argument.");

    if(list_len(args) == 1)
        return new_number(1.0 / car(args)->num_value);

    rv = car(args)->num_value;
    args = cdr(args);

    while(args != val_nil) {
        rv /= car(args)->num_value;
        args = cdr(args);
    }

    return new_number(rv);
}

obj* proc_lt(obj* args) {
    if(args == val_nil || list_len(args) != 2)
        die("< takes 2 arguments.");

    return car(args)->num_value < cadr(args)->num_value ? val_true : val_false;
}

obj* proc_gt(obj* args) {
    if(args == val_nil || list_len(args) != 2)
        die("> takes 2 arguments.");

    return car(args)->num_value > cadr(args)->num_value ? val_true : val_false;
}

obj* proc_eq(obj* args) {
    if(args == val_nil || list_len(args) != 2)
        die("= takes 2 arguments.");

    return car(args)->num_value == cadr(args)->num_value ? val_true : val_false;
}

// List ops

obj* proc_cons(obj* args) {
    if(args == val_nil || list_len(args) != 2)
        die("cons takes 2 arguments.");

    obj* ncar = car(args);
    obj* ncdr = cadr(args);

    return cons(ncar, ncdr);
}

obj* proc_car(obj* args) {
    if(args == val_nil || list_len(args) != 1)
        die("car takes 1 argument.");

    return caar(args);
}

obj* proc_cdr(obj* args) {
    if(args == val_nil || list_len(args) != 1)
        die("cdr takes 1 argument.");

    return cdar(args);
}

obj* proc_set_car(obj* args) {
    if(args == val_nil || list_len(args) != 2)
        die("set-car! takes 2 arguments.");

    obj* ncar = car(args);
    obj* lst = cadr(args);

    lst->pair.car = ncar;
    return lst;
}

obj* proc_set_cdr(obj* args) {
    if(args == val_nil || list_len(args) != 2)
        die("set-cdr! takes 2 arguments.");

    obj* ncdr = car(args);
    obj* lst = cadr(args);

    lst->pair.cdr = ncdr;
    return lst;
}

obj* proc_list(obj* args) {
    return args;
}

// Map ops

obj* proc_get(obj* args) {
    if(args == val_nil || list_len(args) != 2)
        die("get takes 2 arguments.");

    obj* m = car(args);
    obj* key = cadr(args);

    obj* rv = map_get(m->map_value, key);

    if(rv)
        return rv;
    else
        return val_nil;
}

obj* proc_put(obj* args) {
    if(args == val_nil || list_len(args) != 3)
        die("put takes 3 arguments.");

    obj* m = car(args);
    obj* key = cadr(args);
    obj* val = caddr(args);
    obj* rv = clone_obj(m);

    map_put(rv->map_value, key, val);

    return rv;
}
