#include "cell.h"
#include <math.h>
#include <stdio.h>

void array_print(int* a, int size) {
    printf("Array %p: [", a);

    for (int i = 0; i < size; i++) {
        printf("%d", a[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

/*
A cell instance represents a sudoku cell. Each cell has a value which is the
"filled in" value of the cell. The index represents the cell's position in the
sudoku. It can range from 0 to 80. The neighbors array contains the indeces of
cells to which the cell is adjecent (there must exist no two cells that are
adjecent and have the same value). The pencilmakrs stack represents the values
that when filled in the sudoku would still be valid.
*/

/*
This function creates/returns an "empty" cell instance. It had an empty value
and no position in the puzzle. It allocates the appropriate memory for the
neighbors array and the pencilmakrs stack.
*/
Cell* create_cell() {
    Cell* c = (Cell*) malloc(sizeof(Cell));
    c->value = 0;
    c->index = -1;
    c->neighbors = (int*) malloc(sizeof(int) * 20);
    c->pencilmakrs = create_stack();
}

/*
This function frees a cell instance from memory. First it frees the allocated
memory for the neighbors array and the pencilmakrs stack and then frees the
space used by itself.
*/
void free_cell(Cell* c) {
    free(c->neighbors);
    free_stack(c->pencilmakrs);
    free(c);
}

/*
This function creates a sudoku cell with a specific value and position. First
it allocates the necessary memory to create a generic cell and then applies the
specific values. After that it also calculates the indeces of the cells to which
this cell is adjecent.
*/
Cell* add_cell(int val, int index) {
    Cell* c = create_cell();
    c->value = val;
    c->index = index;
    cell_calculate_neighbor_indeces(c);
    return c;
}

/*
This function gets the x component of the position of a cell on the 2d grid grid
that represents the sudoku puzzle.
*/
int cell_calculate_x(Cell* c) {
    return c->index % 9;
}

/*
This function gets the y component of the position of a cell on the 2d grid grid
that represents the sudoku puzzle.
*/
int cell_calculate_y(Cell* c) {
    return (int)(c->index / 9);
}

/*
This function uses the fact that x + y * width is injective. It finds
to which box a cell belongs using the x,y components of its 2d position and
maps that pair to a 1d value. That is used to identify each 3x3 box on the grid
*/
int cell_calculate_box(Cell* c) {
    //dividing by 3 and then multiplying by 3 is redundant
    //but it's more intuitive, this function is only used for setup
    //so we can afford not to optimise to "death"

    int bx = cell_calculate_x(c) / 3;
    int by = cell_calculate_y(c) / 3;

    return bx + by * 3;
}

/*
This function calculates the indeces of the cells to which a cell is adjecent.
Two cells are adjecent if any of their x,y,box components are equal. Also no
cell is adjecent to itself. Each cell should have 20 neighbors. It is to be
called only once when the cell is initialized.
*/
void cell_calculate_neighbor_indeces(Cell* c) {
    //calculate the x,y,box of the cell
    int cx = cell_calculate_x(c);
    int cy = cell_calculate_y(c);
    int cb = cell_calculate_box(c);

    //create a token cell that will be used to calculate the x,y,box components
    //of other cells
    Cell* other = create_cell();

    //a counter to know how many cells have been found
    int counter = 0;

    //loop over all possible indeces
    for (int i = 0; i < 81; i++) {
        //set the index of the token cell to the one we workd with now
        other->index = i;

        //a cell should not be adjecent to itself
        if (other->index == c->index) continue;

        //calculate the x,y,box components of that cell
        int ox = cell_calculate_x(other);
        int oy = cell_calculate_y(other);
        int ob = cell_calculate_box(other);

        //if the two cells have any components that are equal, then
        //they should be adjecent
        if (cx == ox || cy == oy || cb == ob) {
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
This function calculates the pencilmakrs of a cell. Then pencilmakrs of a cell
are affected by the values of its neighbors. A value between [1,...,9] should be
in the pencilmakrs stack iff there exists no neighbor such that it has that value
as a its own value.
*/
void cell_calculate_pencilmarks(Cell* c, Cell** sud) {
    //if the cell is not empty we don't bother with calculating the pencilmakrs
    //at all
    if (c->value != 0) return;

    //remove any values that are already in the pencilmakrs stack
    stack_clear(c->pencilmakrs);
    //for each value in [1,...,9]
    for (int val = 1; val <= 9; val++) {
        //assume the value will go in the pencilmakrs stack
        int still_valid = 1;

        //iterate over all the neighbor indeces
        for (int i = 0; i < 20; i++) {
            //the index of i'th the neighbor
            int n_index = c->neighbors[i];

            //get the cell at that position
            Cell* ne = sud[n_index];

            //if the neighbor has the same value as the one we are currently
            //inspecting
            if (ne->value == val) {
                //this value will not go in the pencilmakrs stack
                still_valid = 0;

                //we dont need to iterate over the rest of neighbors
                break;
            }
        }

        //if it turns out that the value can go in the pencilmakrs stack
        if (still_valid) {
            //push that value in the pencilmakrs stack
            stack_push(c->pencilmakrs, val);
        }
    }
}

int cell_find_unique_pencilmarks(Cell* c, Cell** sud, int* indeces) {

    if (stack_get_size(c->pencilmakrs) < 2) return 0;

    stack* clone = stack_clone(c->pencilmakrs);
    for (int i = 0; i < 9; i++) {
        Cell* n = sud[indeces[i]];
        if (n->index == c->index) continue;
        if (n->value != 0) continue;

        elem* cur = n->pencilmakrs->sp;
        while (cur) {
            stack_remove_all(clone, cur->data);
            cur = cur->next;
        }

        if (stack_is_empty(clone)) break;
    }

    stack* new_pencilmarks;
    stack* to_free;

    if (stack_get_size(clone) == 1) {
        stack_clear(c->pencilmakrs);
        stack_push(c->pencilmakrs, stack_pop(clone));
    }

    free_stack(clone);

    return stack_get_size(c->pencilmakrs) == 1;

}
