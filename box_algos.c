#include "box_algos.h"

void remove_pencilmark_from_rest_of_line(Cell **sud, int *line_indeces, Cell *c1, Cell *c2, int val)
{
    for (int i = 0; i < 9; i++)
    {
        Cell *other = sud[line_indeces[i]];
        if (other == c1 || other == c2)
            continue;

        if (!cell_is_empty(other))
            continue;

        // printf("will remove %d from %d,%d\n", val, cell_calculate_x(other), cell_calculate_y(other));
        pencilmarks_set_remove_pencilmark(other->pencilmakrs, val);
    }
}

void box_find_cells_with_value(Cell **sud, int *box_indeces, int val, stack *to_put)
{

    for (int i = 0; i < 9; i++)
    {
        Cell *c = sud[box_indeces[i]];
        if (!cell_is_empty(c))
            continue;

        if (pencilmarks_set_contains(c->pencilmakrs, val))
        {
            stack_push(to_put, box_indeces[i]);
        }
    }
}

void box_find_pointing_pairs(Sudoku *s, int *box_indeces)
{

    stack *indeces = create_stack();
    for (int val = 1; val <= 9; val++)
    {
        stack_clear(indeces);
        box_find_cells_with_value(s->nodes, box_indeces, val, indeces);

        if (stack_get_size(indeces) == 2)
        {
            Cell *c1 = s->nodes[indeces->sp->data];
            Cell *c2 = s->nodes[indeces->sp->next->data];

            int c1x = cell_calculate_x(c1);
            int c1y = cell_calculate_y(c1);
            int c2x = cell_calculate_x(c2);
            int c2y = cell_calculate_y(c2);

            if (c1x != c2x && c1y != c2y)
                continue;

            // char str1[50];
            // char str2[50];
            // printf("(%d,%d)->%s (%d,%d)->%s\n", c1x, c1y, stack_to_string(c1->pencilmakrs->list, str1), c2x, c2y, stack_to_string(c2->pencilmakrs->list, str2));

            // printf("will remove %d\n", val);

            if (c1x == c2x)
            {
                remove_pencilmark_from_rest_of_line(s->nodes, s->columns[c1x], c1, c2, val);
            }
            else if (c1y == c2y)
            {
                remove_pencilmark_from_rest_of_line(s->nodes, s->rows[c1y], c1, c2, val);
            }
            // getchar();
        }
    }
}
