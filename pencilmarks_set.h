#include "stack.h"

#define ALREADY_SET -10
#define CANNOT_DELETE_UNSET -11
#define SET_EMPTY -1

typedef struct _PencilmarksSet
{
    int mask;
    stack *list;
} PencilmarksSet;

typedef PencilmarksSet PSet;

PSet *pencilmarks_set_create();
void pencilmarks_set_free(PSet *ps);
int pencilmarks_set_contains(PSet *ps, int val);
int pencilmarks_set_get_size(PSet *ps);
int pencilmarks_set_add_pencilmark(PSet *ps, int val);
int pencilmarks_set_remove_pencilmark(PSet *ps, int val);
int pencilmarks_set_clear(PSet *ps);
int pencilmarks_set_pop_value(PSet *ps);
int pencilmarks_set_equals(PSet *ps1, PSet *ps2);
PSet *pencilmarks_set_intersection(PSet *ps1, PSet *ps2);
PSet *pencilmarks_set_difference(PSet *to_keep, PSet *other);
