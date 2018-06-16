#include "pencilmarks_set.h"
#include "utils.h"

PSet *pencilmarks_set_create()
{
    PSet *ps = (PSet *)malloc(sizeof(PSet));
    ps->list = create_stack();
    ps->mask = 0x0;

    return ps;
}

void pencilmarks_set_free(PSet *ps)
{
    free_stack(ps->list);
    free(ps);
}

int pencilmarks_set_contains(PSet *ps, int val)
{
    return get_bit(ps->mask, val);
}

int pencilmarks_set_get_size(PSet *ps)
{
    return stack_get_size(ps->list);
}

int pencilmarks_set_add_pencilmark(PSet *ps, int val)
{
    if (pencilmarks_set_contains(ps, val))
    {
        return ALREADY_SET;
    }

    stack_push(ps->list, val);
    set_bit(&(ps->mask), val, 1);

    return 0;
}

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

    stack_remove(ps->list, val);
    set_bit(&(ps->mask), val, 0);

    return 0;
}

int pencilmarks_set_clear(PSet *ps)
{
    stack_clear(ps->list);
    ps->mask = 0x0;
}

int pencilmarks_set_pop_value(PSet *ps)
{
    int val = stack_peek(ps->list);
    //if the val is a valid pencilmark
    if (val >= 1 && val <= 9)
    {
        pencilmarks_set_remove_pencilmark(ps, val);
    }
    return val;
}

int pencilmarks_set_equals(PSet *ps1, PSet *ps2)
{
    return ps1->mask == ps2->mask;
}

PSet *pencilmarks_set_intersection(PSet *ps1, PSet *ps2)
{
    PSet *intersection = pencilmarks_set_create();
    for (int pval = 1; pval <= 9; pval++)
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
