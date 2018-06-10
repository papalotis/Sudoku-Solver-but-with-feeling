#include "cell.h"
#include <math.h>
#include <stdio.h>

/**
 * A cell instance represents a sudoku cell. Each cell has a value which is the
 * "filled in" value of the cell. The index represents the cell's position in the
 * sudoku. It can range from 0 to 80. The neighbors array contains the indeces of
 * cells to which the cell is adjecent (there must exist no two cells that are
 * adjecent and have the same value). The pencilmakrs stack represents the values
 * that when filled in the sudoku would still be valid.
 */

/*
 * This function creates/returns an "empty" cell instance. It has an empty value
 * and no position in the puzzle. It allocates the appropriate memory for the
 * neighbors array and the pencilmakrs stack.
 */
Cell *create_cell()
{
    Cell *c = (Cell *)malloc(sizeof(Cell));
    c->value = 0;
    c->index = -1;
    c->x = -1;
    c->y = -1;
    c->box = -1;
    c->neighbors = (int *)malloc(sizeof(int) * 20);
    c->pencilmakrs = create_stack();
    return c;
}

/*
 * This function frees a cell instance from memory. First it frees the allocated
 * memory for the neighbors array and the pencilmakrs stack and then frees the
 * space used by itself.
 */
void free_cell(Cell *c)
{
    free(c->neighbors);
    free_stack(c->pencilmakrs);
    free(c);
}

/*
 * This function creates a sudoku cell with a specific value and position. First
 * it allocates the necessary memory to create a generic cell and then applies the
 * specific values. After that it also calculates the indeces of the cells to which
 * this cell is adjecent.
 */
Cell *add_cell(int val, int index)
{
    Cell *c = create_cell();
    c->value = val;
    c->index = index;
    cell_calculate_neighbor_indeces(c);
    return c;
}

/*
 * This function gets the x component of the position of a cell on the 2d grid grid
 * that represents the sudoku puzzle.
 */
int cell_calculate_x(Cell *c)
{
    return c->index % 9;
}

/*
 * This function gets the y component of the position of a cell on the 2d grid grid
 * that represents the sudoku puzzle.
*/
int cell_calculate_y(Cell *c)
{
    return (int)(c->index / 9);
}

/*
 * This function uses the fact that x + y * width is injective. It finds
 * to which box a cell belongs using the x,y components of its 2d position and
 * maps that pair to a 1d value. That is used to identify each 3x3 box on the grid
 */
int cell_calculate_box(Cell *c)
{
    //dividing by 3 and then multiplying by 3 is redundant
    //but it's more intuitive, this function is only used for setup
    //so we can afford not to optimise to "death"

    int bx = cell_calculate_x(c) / 3;
    int by = cell_calculate_y(c) / 3;

    return bx + by * 3;
}

/**
 * This function returns true when a cell is filled in or not
 */
int cell_is_empty(Cell *c)
{
    return c->value == 0;
}

/**
 * This function finds the pencilmark of the cell that is least frequantly 
 * filled in the sudkoku, since it's the most likely to be correct
 */
int cell_retrieve_next_value_from_pencilmarks(Cell *c, int *value_freq)
{
    //if we have no choise we can directly pop the value from the stack
    if (stack_get_size(c->pencilmakrs) < 2)
    {
        return stack_pop(c->pencilmakrs);
    }

    int val = stack_peek(c->pencilmakrs);
    //iterate over the values in the pencilmarks stack
    elem *cur = c->pencilmakrs->sp->next;
    //so long as there are pencilmarks to consider
    while (cur)
    {
        //the pencilamark we are observing
        int candidate_value = cur->data;
        //if its frequency is smaller than the pencilmark
        //we which is the best yet
        if (value_freq[candidate_value] < value_freq[val])
        {
            //then this pencilmark is the best yet
            val = candidate_value;
        }

        cur = cur->next;
    }

    //remove the value from the pencilmarks
    stack_remove_all(c->pencilmakrs, val);

    //return the pencilmark
    return val;
}

/*
 * This function calculates the indeces of the cells to which a cell is adjecent.
 * Two cells are adjecent if any of their x,y,box components are equal. Also no
 * cell is adjecent to itself. Each cell should have 20 neighbors. This function 
 * should only be called once when the cell is initialized called only once 
 * when the cell is initialized.
 */
void cell_calculate_neighbor_indeces(Cell *c)
{
    //calculate the x,y,box of the cell
    int cx = cell_calculate_x(c);
    int cy = cell_calculate_y(c);
    int cb = cell_calculate_box(c);

    //create a token cell that will be used to calculate the x,y,box components
    //of other cells
    Cell *other = create_cell();

    //a counter to know how many cells have been found
    int counter = 0;

    //loop over all possible indeces
    for (int i = 0; i < 81; i++)
    {
        //set the index of the token cell to the one we work with now
        other->index = i;

        //a cell should not be adjecent to itself
        if (other->index == c->index)
            continue;

        //calculate the x,y,box components of that cell
        int ox = cell_calculate_x(other);
        int oy = cell_calculate_y(other);
        int ob = cell_calculate_box(other);

        //if the two cells have any components that are equal, then
        //they should be adjecent
        if (cx == ox || cy == oy || cb == ob)
        {
            //add that index to the neighbors array
            c->neighbors[counter] = i;
            //increament the counter
            counter++;
        }
    }

    //after we are done we can free the memory tha was used by the token cell
    free_cell(other);
}

/*
 * This function calculates the pencilmakrs of a cell. Then pencilmakrs of a cell
 * are affected by the values of its neighbors. A value between [1,...,9] should be
 * in the pencilmakrs stack iff there exists no neighbor such that it has that value
 * as a its own value.
 */
void cell_calculate_pencilmarks(Cell *c, Cell **sud)
{
    //if the cell is not empty we don't bother with calculating the pencilmakrs
    //at all
    if (c->value != 0)
        return;

    //remove any values that are already in the pencilmakrs stack
    stack_clear(c->pencilmakrs);
    //for each value in [1,...,9]
    for (int val = 1; val <= 9; val++)
    {
        //assume the value will go in the pencilmakrs stack
        int still_valid = 1;

        //iterate over all the neighbor indeces
        for (int i = 0; i < 20; i++)
        {
            //the index of the i'th neighbor
            int n_index = c->neighbors[i];

            //the neighbour
            Cell *ne = sud[n_index];

            //if the neighbor has the same value as the one we are currently
            //inspecting
            if (ne->value == val)
            {
                //this value will not go in the pencilmakrs stack
                still_valid = 0;

                //we dont need to iterate over the rest of neighbors
                break;
            }
        }

        //if it turns out that the value can go in the pencilmakrs stack
        if (still_valid)
        {
            //push that value in the pencilmakrs stack
            stack_push(c->pencilmakrs, val);
        }
    }
}

/*
 * This function finds whether an empty cell has a unique pencilmark
 * compared to all the other empty cells of one of its houses (row, column, box)
 * It receives a cell as input, as well as the cell array that defines the sudoku
 * and a list of indeces that are in the same row, column or box as this cell.
 */
int cell_find_unique_pencilmarks(Cell *c, Cell **sud, int *indeces)
{
    //if the cell has at most one pencilmark, then obviously we don't need
    //to consider it
    if (stack_get_size(c->pencilmakrs) < 2)
        return 0;

    //make a copy of the pencilmarks stack
    stack *clone = stack_clone(c->pencilmakrs);
    //for every cell that is in that house
    for (int i = 0; i < 9; i++)
    {
        Cell *n = sud[indeces[i]];
        //if it's the same cell as the one we are observing then we can ignore it
        if (n->index == c->index)
            continue;
        //if it's filled in then we also ignore it
        if (n->value != 0)
            continue;

        //start at the head of that cells pencilmarks
        elem *cur = n->pencilmakrs->sp;
        //so long as there are elements to consider
        while (cur != NULL)
        {
            //remove all the instances of that elements value
            //from the cloned list
            stack_remove_all(clone, cur->data);
            //move to the next element
            cur = cur->next;
        }

        //if all the elements have been removed from the cloned stack
        //then we can safely assume that we can break out of the loop
        //since no other elements can be removed, and no elements can be
        //added in this loop
        if (stack_is_empty(clone))
            break;
    }
    //if in the end the cloned stack has exactly one value
    //that means that this cell, has a unique pencilmark in this house
    if (stack_get_size(clone) == 1)
    {
        //remove all the elements from the pencilmarks stack of that cell
        stack_clear(c->pencilmakrs);
        //and push the value that still is in the cloned stack to it
        stack_push(c->pencilmakrs, stack_pop(clone));
    }

    //free the cloned stack from memory
    free_stack(clone);

    //and return true if anything changed in the stack of that cell
    return stack_get_size(c->pencilmakrs) == 1;
}

/*
 * This function finds whether two cells of the same house have the exact same
 * two pencilmarks. If that's the case then these two values can be removed from
 * all the other cells of that house
 */
void cell_find_naked_pairs(Cell *c, Cell **sud, int *indeces)
{
    //we only care about cells that don't have a value
    if (c->value != 0)
        return;
    //for a cell to be a valid naked pair it needs to have exactly two pencilmakrs
    if (stack_get_size(c->pencilmakrs) != 2)
        return;

    //for every cell in the house
    for (int i = 0; i < 9; i++)
    {
        Cell *other = sud[indeces[i]];

        //ignore the cell itself in the house
        if (c->index == other->index)
            continue;
        //if the cell is not empty ignore it
        if (other->value != 0)
            continue;
        //if the cell also doesn't have two pencilmakrs ignore it
        if (stack_get_size(other->pencilmakrs) != 2)
            continue;

        //if we found a naked pair
        if (stack_equals(c->pencilmakrs, other->pencilmakrs))
        {
            //we can remove the values from all the other cells
            int val1 = stack_peek(c->pencilmakrs);
            int val2 = c->pencilmakrs->sp->next->data;

            //for every cell in the house
            for (int i = 0; i < 9; i++)
            {
                //get the cell
                Cell *to_remove_from = sud[indeces[i]];
                //and its index
                int index = to_remove_from->index;
                //if the cell has the same index as one of the two naked
                //pair cells, then ignore it
                if (index == c->index || index == other->index)
                    continue;
                //if the cell is not empty also ignore it
                if (to_remove_from->value != 0)
                    continue;

                //remove all the instances of those two values
                //from its pencilmarks stack
                stack_remove_all(to_remove_from->pencilmakrs, val1);
                stack_remove_all(to_remove_from->pencilmakrs, val2);
            }
        }
    }
}

int cell_calculate_error(Cell *c, Cell **sud)
{
    int err = 0;
    for (int i = 0; i < 20; i++)
    {
        Cell *n = sud[c->neighbors[i]];
        if (c->value == n->value)
            err++;
    }

    return err;
}
