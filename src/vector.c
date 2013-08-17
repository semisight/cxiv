#include "vector.h"

#include <stdlib.h>
#include <string.h>

//Constructor
vector* new_vector() {
	vector *v = malloc(sizeof(vector));

	v->arr = malloc(INITIAL_SIZE * sizeof(int));
	v->max = INITIAL_SIZE;
	v->len = 0;

	return v;
}

void del_vector(vector* v) {
	free(v->arr);
}

//Operations
void vec_append(vector* v, int item) {
	if(!v)
		return;

	//Check array bounds
	if(v->len == v->max) {
		v->max *= 2;
		v->arr = realloc(v->arr, v->max * sizeof(int));
	}

	v->arr[v->len++] = item;
}

int vec_pop(vector *v) {
	int rv = vec_get(v, v->len-1);

	if(v->len > 0)
		v->len--;

	return rv;
}

void vec_remove(vector* v, int index) {
	if(pvec_range_check(v, index))
		return;

	if(index == v->len-1)
		vec_pop(v);
	else {
		memmove(v->arr + index, v->arr + index + 1, (v->len - index - 1) * sizeof(int));
	}

	v->len--;
}

//Iteration
int vec_size(vector* v) {
	return v->len;
}

int vec_get(vector* v, int index) {
	if(pvec_range_check(v, index))
		return (unsigned int)-1;

	return v->arr[index];
}

//Private functions
int pvec_range_check(vector* v, int index) {
	return index < 0 || index >= v->len;
}
