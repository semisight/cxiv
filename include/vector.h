#ifndef VECTOR_H__
#define VECTOR_H__

//Constants
#define INITIAL_SIZE 1

typedef struct {
    int* arr;
    int len, max;
} vector;

//Constructor
vector* new_vector();

//Destructor
void del_vector(vector*);

//Operations
void vec_append(vector*, int);
int vec_pop(vector*);
void vec_remove(vector*, int);
void vec_put(vector*, int, int);

//Getters
int vec_size(vector*);
int vec_get(vector*, int);

//Private functions
int pvec_range_check(vector*, int);

#endif /* VECTOR_H__ */
