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
    return count_ones(ps->mask);
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
    set_bit(&(ps->mask), val, 0);
    return 0;
}

/**
 * Removes all the values from a set
 */
int pencilmarks_set_clear(PSet *ps)
{
    //Set the mask to zero again
    ps->mask = EMPTY_MASK;
}

/**
 * Get the first value from the stack
 * that represents the set
 */
int pencilmarks_set_pop_value(PSet *ps)
{

    int val = trailing_zeros(ps->mask);
    ps->mask ^= (1 << val);

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
int pencilmarks_set_intersection(PSet *ps1, PSet *ps2)
{

    return ps1->mask & ps2->mask;
}

/**
 * Returns a new set that contains the elements
 * that are only in ps1 and not in ps2
 * THIS FUNCTION ALLOCATES MEMORY THAT NEEDS TO BE 
 * FREED LATER
 */
int pencilmarks_set_difference(PSet *to_keep, PSet *other)
{

    return to_keep->mask & ~other->mask;
}
