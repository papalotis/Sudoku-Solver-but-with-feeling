#include "stack.h"
#include <stdio.h>
#include <string.h>

#define STACK_EMPTY -1

/*
 * A generic stack structure that holds non negative numbers.
 * It has a stack pointer that points to the head stack element.
 * The size indicates how many elements are stored in the stack.
 * 
 * The element structure has a data field that holds the data and a next field
 * that holds its next stack element
 */

/*
 * This function creates an empty stack
 */
stack *create_stack()
{
    //allocate the memory needed by the stack
    stack *s = (stack *)malloc(sizeof(stack));
    //set the sp to point at "nothing"
    s->sp = NULL;
    //the stack is empty so the size is 0
    s->size = 0;
    //return the stack
    return s;
}

/*
 * This function creates an isolated element with a specific value
 */
elem *create_elem(int n)
{
    //allocate the memory needed by the element itself
    elem *e = (elem *)malloc(sizeof(elem));
    //set the data of the element the same as the given data
    e->data = n;
    //the element is isolated so it has no next element
    e->next = NULL;
    //return the newly created element
    return e;
}

/*
 * This function frees a stack from memory. First it deletes/frees all the
 * elements it the stack using the clear function and then frees the memory
 * it uses itself
 */
void free_stack(stack *s)
{
    stack_clear(s);
    free(s);
}

/*
 * This function frees the memory that is used by a stack element.
 * Since it doesn't allocate any extra memory, it only needs to free itself
 */
void free_elem(elem *e)
{
    free(e);
}

/*
 * This function pushes a value to the stack
 */
void stack_push(stack *s, int n)
{
    //first get the current stack pointer
    elem *next = s->sp;
    //create the new element with the given value
    elem *new = create_elem(n);
    //set the stack pointer to point at the new element
    s->sp = new;
    //point the next of the new stack pointer to the previous one
    s->sp->next = next;
    //increase the size of the stack by one
    s->size += 1;
}

/*
 * This function pushes many values to a stack
 */
void stack_push_many(stack *s, int *data, int len)
{
    //for every value in the data array
    for (int i = 0; i < len; i++)
    {
        //push it to the stack
        stack_push(s, data[i]);
    }
}

/**
 * This function creates a new stack
 * that contains the exact same elements
 * as the input stack
 */
stack *stack_clone(stack *s)
{
    //create an empty stack
    stack *copy = create_stack();
    int vals[s->size];

    //get the head of the input stack
    elem *cur = s->sp;

    //we will be putting the values
    //of the stack to an array, and then
    //push the values all at once from the array.
    //we will be putting the values backwards in the array
    //to address the lifo nature of the stack
    int counter = s->size - 1;
    //for every element in the stack
    while (cur)
    {
        //put the value of the stack elements in the array
        vals[counter--] = cur->data;
        //and move to the next element
        cur = cur->next;
    }
    //push the values we got from the stack
    //to the new stack
    stack_push_many(copy, vals, s->size);

    //and return the stack we created
    return copy;
}

/*
 * This function returns the value of the stack pointer
 */
int stack_peek(stack *s)
{
    //so long as the stack exists and the stack is not empty
    if (s && !stack_is_empty(s))
    {
        //return the value of the head
        return s->sp->data;
    }

    //otherwise indicate that the stack is empty to the caller
    return STACK_EMPTY;
}

/*
 *This function pops/removes the head of the stack and returns its value
 */
int stack_pop(stack *s)
{
    //if the stack is empty an underflow would occur, indicate
    //to the caller that the stack is empty
    if (stack_is_empty(s))
        return STACK_EMPTY;

    //get the value of the head
    int r = stack_peek(s);
    //store the current head
    elem *prev = s->sp;
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
 * This function removes the first element with a specific value
 */
int stack_remove(stack *s, int val)
{
    //holds the element we were looking at previously
    elem *prev = NULL;
    //holds the element that we are inspecting
    //(starting with the head of the stack)
    elem *cur = s->sp;
    //so long as there is an element to inspect
    while (cur != NULL)
    {
        //if the element should be removed
        if (cur->data == val)
        {
            //if we need to remove the head of the list
            if (prev == NULL)
            {
                //set the stack pointer to point the next of the head
                s->sp = cur->next;
            }
            //otherwise if it's another element
            else
            {
                //set the next of the previous element to equal
                //the next of the current element
                prev->next = cur->next;
            }

            //we removed an element so decreament the size of the stack by one
            s->size += -1;
            //free the element we want to remove
            free_elem(cur);

            //and return 1 in order to indicate that an element was removed
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }

    //if we are out of the loop that means that no element should be removed
    //return 0 to indicate that no element was removed
    return 0;
}

/*
 * This function removes the all the elements with a specific value
 * and returns how many elements were removed
 */
int stack_remove_all(stack *s, int val)
{
    //holds how many elements have been removed
    int count = 0;
    //holds the result of the remove_one function
    int rem = 1;
    //so long as we have removed another element
    while (rem)
    {
        //try to remove another one
        rem = stack_remove(s, val);
        //increament the total count
        count += rem;
    }
    //return how many elements were removed
    return count;
}

/*
 * This function deletes all the elements from a stack
 */
void stack_clear(stack *s)
{
    //run this "forever"
    while (1)
    {
        //remove an element from the stack
        stack_pop(s);
        //if the stack is empty break out of the loop
        if (stack_is_empty(s))
            break;
    }
}

/*
 * This function returns the size of the stack
 */
int stack_get_size(stack *s)
{
    return s->size;
}

/*
 * This function returns true when a stack is empty
 * A stack is empty when its size is 0
 */
int stack_is_empty(stack *s)
{
    return stack_get_size(s) == 0;
}

/**
 * This function "converts" a stack to a string
 * It receives a stack a buffer as arguments.
 * The function writes the contents of the stack
 * to the buffer and in the end returns the filled in
 * buffer. If no buffer is provider the function 
 * creates one for itself.
 */
char *stack_to_string(stack *s, char *buff)
{
    //only do something is the stack exists
    if (!s)
        return NULL;

    //how many chars we will need
    //for now this is an overestimation
    int str_len = 20 + 4 * stack_get_size(s);

    //if the caller didn't provide a buffer to write
    //we create one for ourselves.
    //WARNING: This needs to be freed by someone but not us
    if (buff == NULL)
    {
        buff = (char *)malloc(str_len * sizeof(char));
    }

    //reset the buffer
    memset(buff, 0, str_len);

    char num_str[3];
    //some helper constant strings
    char *commaspace = ", ";
    char *prefix = "Stack %p: [";
    char *closingbracket = "]";

    //add the prefix to the buffer
    sprintf(buff, prefix, s);

    //start at the head of the stack
    elem *cur = s->sp;
    //so long as the cur is a valid element
    while (cur != NULL)
    {
        //convert the value of the element to a string
        sprintf(num_str, "%d", cur->data);

        //and add it to the buffer
        strcat(buff, num_str);

        //go the next element
        cur = cur->next;
        //if there is another element after the one that was printed
        //print a comma for readability
        if (cur != NULL)
            strcat(buff, commaspace);
    }

    //add a closing bracket after all the elements have been written
    //to the buffer
    strcat(buff, closingbracket);

    //return the buffer
    return buff;
}

/*
 * This function prints the data of the stack to stdout
 */
void stack_print(stack *s)
{
    //convert the stack to a string
    char *stack_str = stack_to_string(s, NULL);
    //print the string
    printf("%s\n", stack_str);
    //free the allocated memory used by the buffer
    free(stack_str);
}

/*
 * This function returns true if there is an elemtent in the stack
 * that has the specific value, otherwise false
 */
int stack_contains(stack *s, int val)
{
    //start with the top of the stack
    elem *e = s->sp;
    //so long as there are still elements to consider
    while (e != NULL)
    {
        //if that element has the same value as the one we are looking for
        //we can immediatelly return one
        if (e->data == val)
            return 1;
        //otherwise move to the next element
        e = e->next;
    }
    //we traversed through the entire stack and couldn't find
    //an element with that value so we can safely return false
    return 0;
}

/**
 * This function returns true, if all the elements of two stacks are equal
* 1-1.
* stack_equals([3,1,5], [3,1,5]) -> true
* stack_equals([3,5,1], [3,1,5]) -> false
* stack_equals([3], [3,5]) -> false
*/
int stack_equals(stack *s1, stack *s2)
{
    //if two stacks are of different size then they are obviously not equal
    if (stack_get_size(s1) != stack_get_size(s2))
        return 0;

    //declare two elem variables, that will each traverse each stack
    elem *e1, *e2;
    //they both start at the head of each stack
    e1 = s1->sp;
    e2 = s2->sp;
    //so long as there are elements to consider
    while (e1 != NULL)
    {
        //if there are two elements that are not equal we can safely return false
        if (e1->data != e2->data)
            return 0;
        //otherwise we need to consider the next elemtent of each stack
        e1 = e1->next;
        e2 = e2->next;
    }
    //we went through both stacks completely and could not find an unequal pair
    //therefore the stacks must be equal
    return 1;
}
