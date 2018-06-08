
#if !defined(STACK_H)
#define STACK_H

#include <stdlib.h>

typedef struct _elem {
    int data;
    struct _elem *next;
} elem;

typedef struct _stack {
    elem* sp;
    int size;
} stack;


stack* create_stack();
elem* create_elem(int n);
void free_stack(stack* s);
void free_elem(elem* e);
void stack_push(stack* s, int n);
void stack_push_many(stack* s, int* data, int len);
stack* stack_clone(stack* s);
int stack_peek(stack* s);
int stack_pop(stack* s);
int stack_remove(stack* s, int val);
int stack_remove_all(stack* s, int val);
void stack_clear(stack* s);
int stack_get_size(stack* s);
int stack_is_empty(stack* s);
void stack_print(stack* s);
int stack_contains(stack* s, int val);
int stack_equals(stack* s1, stack* s2);


#endif // STACK_H

