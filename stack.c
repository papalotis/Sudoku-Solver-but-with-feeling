#include "stack.h"
#include <stdio.h>

#define STACK_EMPTY -1

/*
A generic stack structure that holds non negative numbers.
It has a stack pointer that points to the head stack element.
The size indicates how many elements are stored in the stack.

The element structure has a data field that holds the data and a next field
that holds its next stack element
*/

/*
This function creates an empty stack
*/
stack* create_stack() {
    //allocate the memory needed by the stack
    stack* s = (stack*) malloc(sizeof(stack));
    //set the sp to point at "nothing"
    s->sp = NULL;
    //the stack is empty so the size is 0
    s->size = 0;
    //return the stack
    return s;
}

/*
This function creates an isolated element with a specific value
*/
elem* create_elem(int n) {
    //allocate the memory needed by the element itself
    elem* e = (elem*) malloc(sizeof(elem));
    //set the data of the element the same as the given data
    e->data = n;
    //the element is isolated so it has no next element
    e->next = NULL;
    //return the newly created elemtent
    return e;
}

/*
This function frees a stack from memory. First it deletes/frees all the
elements it the stack using the clear function and then frees the memory
it uses itself
*/
void free_stack(stack* s) {
    stack_clear(s);
    free(s);
}

/*
This function frees the memory that is used by a stack element
Since it doesn't allocate any extra memory, it only needs to free itself
*/
void free_elem(elem* e) {
    free(e);
}

/*
This function pushes a value to the stack
*/
void stack_push(stack* s, int n) {
    //first get the current stack pointer
    elem* next = s->sp;
    //create the new element with the given value
    elem* new = create_elem(n);
    //set the stack pointer to point at the new element
    s->sp = new;
    //point the next of the new stack pointer to the previous one
    s->sp->next = next;
    //increase the size of the stack by one
    s->size += 1;
}

/*
This function returns the value
*/
int stack_peek(stack* s) {
    //so long as the stack exists and the stack is not empty
    if (s && !stack_is_empty(s)) {
        //return the value of the head
        return s->sp->data;
    }

    //otherwise return -1
    return STACK_EMPTY;
}

/*
This function pops/removes the head of the stack and returns its value
*/
int stack_pop(stack* s) {
    //if the stack is empty an underflow would occur, return -1
    if (stack_is_empty(s)) return STACK_EMPTY;

    //get the value of the head
    int r = stack_peek(s);
    //store the current head
    elem* prev = s->sp;
    //move the head to point to the next element
    s->sp = s->sp->next;
    //free the previous head
    free_elem(prev);
    //decreament the size of the stack by one
    s->size += -1;
    //return the value of the old head
    return r;


}

/*
This function deletes all the elements from the stack
*/
void stack_clear(stack* s) {
    //run this "forever"
    while(1) {
        //remove an element from the stack
        stack_pop(s);
        //if the stack is empty break out of the loop
        if (stack_is_empty(s)) break;
    }
}

/*
This function returns the size of the stack
*/
int stack_get_size(stack* s) {
    return s->size;
}

/*
A stack is empty when its size is 0
*/
int stack_is_empty(stack* s) {
    return stack_get_size(s) == 0;
}

/*
This function prints the data of the stack to stdouts
*/
void stack_print(stack* s) {
    //print a head
    printf("Stack %p: [", s);
    //start at the head of the stack
    elem* cur = s->sp;
    //so long as the cur is a valid element
    while (cur != NULL) {
        //print the data of the element
        printf("%d", cur->data);
        //go the next element
        cur = cur->next;
        //if there is another element after the one that was printed
        //print a comma for readability
        if (cur != NULL) printf(", ");
    }
    //closing bracket for readability
    printf("]\n");
}
