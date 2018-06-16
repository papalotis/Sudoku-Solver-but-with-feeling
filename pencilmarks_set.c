#include "pencilmarks_set.h"
#include "utils.h"

#define EMPTY_MASK 0x0

/**
 * Returns an empty pencilmarks set. 
 * Allocated memory for the set itself and the stack itself
 * Set the mask so that no numbers are in the set
 */
PSet *pencilmarks_set_create()
{
    PSet *ps = (PSet *)malloc(sizeof(PSet));
    ps->list = create_stack();
    ps->mask = EMPTY_MASK;

    return ps;
}

/**
 * Frees a set from memory 
 * Frees the stack and then the memory used by
 * the set itself
 */
void pencilmarks_set_free(PSet *ps)
{
    free_stack(ps->list);
    free(ps);
}

/**
 * Returns true if a value is in the given set
 */
int pencilmarks_set_contains(PSet *ps, int val)
{
    return get_bit(ps->mask, val);
}

/**
 * Returns true if ps2 only contains elements that 
 * are also in ps1
 */
int pencilmarks_set_is_subset(PSet *ps1, PSet *ps2)
{
    return (ps1->mask | ps2->mask) == ps1->mask;
}

/**
 * Returns how many elements are in a set
 */
int pencilmarks_set_get_size(PSet *ps)
{
    return stack_get_size(ps->list);
}

int pencilmarks_set_get_min(PSet *ps)
{
    return trailing_zeros(ps->mask);
}

int pencilmarks_set_get_max(PSet *ps)
{
    return leading_zeros(ps->mask);
}

/**
 * Adds a value to the set.
 * A set can contain a value only once so 
 * if a value is given to be added that is 
 * already in the set then nothing happens
 */
int pencilmarks_set_add_pencilmark(PSet *ps, int val)
{
    if (pencilmarks_set_contains(ps, val))
    {
        return ALREADY_SET;
    }
    //push the value to the stack
    stack_push(ps->list, val);
    //set the ith bit to 1 to indicate that the value is in
    set_bit(&(ps->mask), val, 1);

    return 0;
}

/**
 * Removes a value from the set. 
 * If a value is to be removed but not in 
 * the set then nothing happens
 */
int pencilmarks_set_remove_pencilmark(PSet *ps, int val)
{
    if (pencilmarks_set_get_size(ps) == 0)
    {
        return SET_EMPTY;
    }

    if (!pencilmarks_set_contains(ps, val))
    {
        return CANNOT_DELETE_UNSET;
    }

    //remove the value from the pencilmarks stack
    //O(n)
    stack_remove(ps->list, val);
    //set the corresponding bit to 0
    //to indicate that the value
    //is no longer in the set
    set_bit(&(ps->mask), val, 0);

    return 0;
}

/**
 * Removes all the values from a set
 */
int pencilmarks_set_clear(PSet *ps)
{
    //Remove all the elements from the stack
    stack_clear(ps->list);
    //Set the mask to zero again
    ps->mask = EMPTY_MASK;
}

/**
 * Get the first value from the stack
 * that represents the set
 */
int pencilmarks_set_pop_value(PSet *ps)
{
    //get the value of the top element of the stack
    int val = stack_peek(ps->list);
    //if the val is a valid pencilmark
    if (val >= 1 && val <= 9)
    {
        //remove that pencilmark from the stack
        pencilmarks_set_remove_pencilmark(ps, val);
    }
    //and return the value
    return val;
}

/**
 * Returns true when two pencilmarks have the exact same values 
 * in them. This can be done just by comparing the masks
 */
int pencilmarks_set_equals(PSet *ps1, PSet *ps2)
{
    return ps1->mask == ps2->mask;
}

/**
 * Returns a new set that contains the elements
 * that are in both sets
 * THIS FUNCTION ALLOCATES MEMORY THAT NEEDS TO BE 
 * FREED LATER
 */
PSet *pencilmarks_set_intersection(PSet *ps1, PSet *ps2)
{

    int start = min(pencilmarks_set_get_min(ps1), pencilmarks_set_get_min(ps2));
    int end = max(pencilmarks_set_get_max(ps1), pencilmarks_set_get_max(ps2));

    PSet *intersection = pencilmarks_set_create();
    for (int pval = start; pval <= end; pval++)
    {
        int in_set_1 = pencilmarks_set_contains(ps1, pval);
        int in_set_2 = pencilmarks_set_contains(ps2, pval);
        if (in_set_1 && in_set_2)
        {
            pencilmarks_set_add_pencilmark(intersection, pval);
        }
    }

    return intersection;
}

/**
 * Returns a new set that contains the elements
 * that are only in ps1 and not in ps2
 * THIS FUNCTION ALLOCATES MEMORY THAT NEEDS TO BE 
 * FREED LATER
 */
PSet *pencilmarks_set_difference(PSet *to_keep, PSet *other)
{
    PSet *difference = pencilmarks_set_create();
    for (int pval = 1; pval <= 9; pval++)
    {
        int in_set_we_want_to_keep = pencilmarks_set_contains(to_keep, pval);
        int in_set_other = pencilmarks_set_contains(other, pval);
        if (in_set_we_want_to_keep && !in_set_other)
        {
            pencilmarks_set_add_pencilmark(difference, pval);
        }
    }

    return difference;
}
