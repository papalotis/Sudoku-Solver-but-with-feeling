#include "cell.h"
#include "utils.h"

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
    c->pencilmakrs = pencilmarks_set_create();
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
    pencilmarks_set_free(c->pencilmakrs);
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
    if (pencilmarks_set_get_size(c->pencilmakrs) < 2)
    {
        return pencilmarks_set_pop_value(c->pencilmakrs);
    }

    int val = stack_peek(c->pencilmakrs->list);
    //iterate over the values in the pencilmarks stack
    elem *cur = c->pencilmakrs->list->sp->next;
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
    pencilmarks_set_remove_pencilmark(c->pencilmakrs, val);

    //return the pencilmark
    return val;
}

/*
 * This function calculates the indeces of the cells to which a cell is adjecent.
 * Two cells are adjecent if any of their x,y,box components are equal. Also no
 * cell is adjecent to itself. Each cell should have 20 neighbors. This function 
 * should only be called once when the cell is initialized.
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
    pencilmarks_set_clear(c->pencilmakrs);
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
            pencilmarks_set_add_pencilmark(c->pencilmakrs, val);
        }
    }
}

/*
 * This function finds whether an empty cell has a unique pencilmark
 * compared to all the other empty cells of one of its houses (row, column, box)
 * It receives a cell as input, as well as the cell array that defines the sudoku
 * and a list of indeces that are in the same row, column or box as this cell.
 */
int cell_find_unique_pencilmarks(Cell *c, Cell **sud, int *house_indeces)
{
    //if the cell has at most one pencilmark, then obviously we don't need
    //to consider it
    if (pencilmarks_set_get_size(c->pencilmakrs) < 2)
        return 0;

    int penclimarks_mask = c->pencilmakrs->mask;

    //for every cell that is in that house
    for (int i = 0; i < 9; i++)
    {
        Cell *n = sud[house_indeces[i]];
        //if it's the same cell as the one we are observing then we can ignore it
        if (n->index == c->index)
            continue;
        //if it's filled in then we also ignore it
        if (n->value != 0)
            continue;

        //make every position that a neighboring cell contains a
        //pencilmark a zero
        penclimarks_mask &= ~(n->pencilmakrs->mask);

        if (penclimarks_mask == 0)
            break;
    }

    //if in the end the cloned stack has exactly one value
    //that means that this cell, has a unique pencilmark in this house
    if (is_power_of_two(penclimarks_mask))
    {
        //get the unique pencilmark
        int unique_pencilmark = trailing_zeros(penclimarks_mask);
        //remove all the values from the pencilmarks set
        pencilmarks_set_clear(c->pencilmakrs);
        //add the unique pencilmark
        pencilmarks_set_add_pencilmark(c->pencilmakrs, unique_pencilmark);
    }

    //and return true if anything changed in the stack of that cell
    return pencilmarks_set_get_size(c->pencilmakrs) == 1;
}

void cell_find_naked_partners(Cell *c, Cell **sud, int *house_indeces)
{
    //if the cell is not empty then we don't want to bother with it
    if (!cell_is_empty(c))
        return;

    //if the cell doesn't have any ambigiouity then there is no point finding
    //partenrs for it
    if (pencilmarks_set_get_size(c->pencilmakrs) < 2)
        return;

    //if the cell has more than 3 cells, the chances that it will
    //have another cell in the house with the same pencilmarks is
    //very small, so cutoff from 3
    // if (stack_get_size(c->pencilmakrs) > 3)
    //     return;

    //the cells that have the exact same pencilmarks
    Cell *same_pencilmarks[9];
    int num_same;
    //the cells that are empty and don't have the exact same pencilmarks as c
    Cell *complements[9];
    int num_comp;

    cell_get_neighbours_with_same_pencilmarks_in_house(c,                /*the cell*/
                                                       sud,              /*the sudoku cells array*/
                                                       house_indeces,    /*the indeces of the cells in this house*/
                                                       same_pencilmarks, /*the buffer were the cells that have 
                                                       the same pencilmarks as c will be put*/
                                                       &num_same,        /*the number of cells that have the same pencilmarks will be written in this variable*/
                                                       complements,      /*the cells that are empty and don't have the same pencilmarks as c will be put in here*/
                                                       &num_comp);       /*the number of cells in the complements buffer*/

    //if we didn't find as many valid cells as we have pencilmarks
    //then we can't do much more
    if (num_same != pencilmarks_set_get_size(c->pencilmakrs) - 1)
        return;

    //for every complement cell, remove our pencilmarks from their pencilmarks
    for (int i = 0; i < num_comp; i++)
    {
        Cell *to_remove_from = complements[i];

        elem *cur = c->pencilmakrs->list->sp;
        while (cur)
        {
            int val = cur->data;
            pencilmarks_set_remove_pencilmark(to_remove_from->pencilmakrs, val);
            cur = cur->next;
        }
    }
}

/**
 * This function returns the number of neighbors in a house that have the exact same
 * pencilmarks as a given cell
 */
void cell_get_neighbours_with_same_pencilmarks_in_house(Cell *c, Cell **sud, int *house_indeces, Cell **same_buffer,
                                                        int *same_num, Cell **comp_buffer, int *comp_num)
{
    int same_counter = 0;
    int comp_counter = 0;

    PSet *our_pencilmarks = c->pencilmakrs;
    PSet *other_pencilmarks = NULL;
    //for every cell in the house
    for (int i = 0; i < 9; i++)
    {

        Cell *other = sud[house_indeces[i]];
        //if the other cell is the same as the main one ignore it
        if (other == c)
            continue;
        //if the other cell has a value in it, ignore it
        if (!cell_is_empty(other))
            continue;

        other_pencilmarks = other->pencilmakrs;

        //if the main cell and the other one have the
        //exact same pencilmarks, then add the other
        //pencilmark to the array containing the cells
        //that have the exact same pencilmarks as the main cell
        if (pencilmarks_set_equals(our_pencilmarks, other_pencilmarks))
        {
            same_buffer[same_counter++] = other;
        }
        else
        {
            //otherwise add it to the compliments array
            //this means that the cell is empty and has
            //at least one different pencilmark, or
            //doesn't have as many pencilmarks
            comp_buffer[comp_counter++] = other;
        }
    }

    *same_num = same_counter;
    *comp_num = comp_counter;
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
