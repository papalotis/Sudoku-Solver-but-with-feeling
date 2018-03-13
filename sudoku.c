#include "sudoku.h"
#include <stdio.h>

#define INDEX_UNINITIALIZED -2
#define NO_VALID_POS -1



/*
A sudoku instance represents a sudoku puzzle at a specific state. It contains a
sudoku cell array (nodes) that represents all the cells that a sudoku is comprised of.
It has an indeces stack that is used to hold the backtracking path in which we
are. In has a size that represents the size of the sudoku puzzle.
nextIndex represents the index of the cell that is at the end of the backtracking
path.
*/


/*
This function creates an empty sudoku puzzle. It sets the size to 81 (as in a
9x9 grid). It allocates the necessary memory for the nodes array and the indeces
stack, and sets nextIndex to the appropriate value
*/
Sudoku* create_sudoku() {
    Sudoku* s = (Sudoku*) malloc(sizeof(Sudoku));
    s->size = 81;
    s->nodes = (Cell**) malloc(sizeof(Cell*) * s->size);
    s->indeces = create_stack();
    s->nextIndex = INDEX_UNINITIALIZED;
    return s;
}

/*
This function frees a sudoku instance from memory. First it frees the memory used
by each cell that defines the sudoku puzzle. Then it frees the nodes array and
the indeces stack and lastly it frees the memory used by itself
*/
void free_sudoku(Sudoku* s) {
    for (int i = 0; i < s->size; i++) {
        free_cell(s->nodes[i]);
    }
    free(s->nodes);
    free_stack(s->indeces);
    free(s);
}

/*
This function prints a sudoku instance at the state in which it is.
A cell is "empty" if it has the value 0 in it.
*/
void sudoku_print(Sudoku* s) {
    //for every cell
    for (int i = 0; i < s->size; i++) {
        //the dashes and pipes that are printed are done so as to
        //differentiate between boxes

        //if we are at the beginnig of the forth or sevent line
        if (i == 27 || i == 54) {
            //print a line full of dashes
            printf("------+-------+------\n");
        }

        //after every three cells printed in a row
        if (i % 9 == 3 || i % 9 == 6) {
            //print a pipe
            printf("| ");
        }

        //get the value of a cell
        int val = s->nodes[i]->value;
        //for a value different from 0, we print that value
        if (val > 0)
            printf("%d", val);

        //otherwise we print an underscore to show that, that cell
        //needs to be filled in
        else
            printf("_");

        //after each cell if printed, print an empty character to make
        //the end result more readable
        printf(" ");

        //after printing 9 cells print a newline so as to differentiate
        //between rows
        if (i % 9 == 8) printf("\n");
    }
}


/*
This function creates a sudoku puzzle and fills it in with the given data
*/
Sudoku* sudoku_create_from(int* data) {
    //crete an empty sudoku instance
    Sudoku* s = create_sudoku();
    //for each value in the data
    for (int i = 0; i < s->size; i++) {
        //create a cell with the specific data and index and
        //add it to the nodes array
        s->nodes[i] = add_cell(data[i], i);
    }
    //calculate the pencilmakrs of all the nodes of the new sudoku
    sudoku_calculate_pencilmarks(s);
    //initialize the nextIndex value
    s->nextIndex = sudoku_find_next_index(s);

    //return the created sudoku
    return s;
}

/*
This function runs one step of the solving algorithm
*/
int sudoku_solve_step(Sudoku* s) {
    //the value that will be returned
    //assume we will keep on solving
    int r = SUDOKU_UNDECIDED;

    //get the cell that we need to find the next value
    Cell* c = s->nodes[s->nextIndex];

    //get the next value from the pencilmakrs stack of that cell
    int val = stack_pop(c->pencilmakrs);
    //clean up the result of the stack (if it's negative that means the stack
    //was empty)
    c->value = val > 0 ? val : 0;

    //if there is no value so that the sudoku is still valid
    if (c->value == 0) {
        //pop the last value from the indeces stack
        //we move upwards in the backtracking tree
        if (!stack_is_empty(s->indeces)) {
            s->nextIndex = stack_pop(s->indeces);
        }
        else {
            r = SUDOKU_NO_SOLUTUION;
        }
    }

    //if we found a value such that the sudoku is still valid
    else {
        //push the current index to the indeces array
        //we move downwards in the backtracking tree
        stack_push(s->indeces, s->nextIndex);

        //calculate the index of the next cell that we will try to fill in
        s->nextIndex = sudoku_find_next_index(s);

        if (s->nextIndex == NO_VALID_POS)
            r = SUDOKU_SOLVED;

        //and calculate the pencilmakrs of the sudoku
        sudoku_calculate_pencilmarks(s);
    }

    return r;

}

/*
This function runs the sudoku solving algorithm up until the sudoku is solved
*/
int sudoku_solve(Sudoku* s) {
    //assume that we will need to run at least one more step
    int r = SUDOKU_UNDECIDED;
    //while the sudoku is not solved or it has NOT been determined that
    //there is no solution
    while(r == SUDOKU_UNDECIDED) {
        //run a step of the solution algorithm
        //and get its decision
        r = sudoku_solve_step(s);
    }

    //return whether the sudoku was solved or has no solution
    return r;
}


/*
This function determines the index of the cell that we will run the
algorithm next on. The decision is made based on the size of the pencilmakrs
stack of each cell. The index of the cell that we want to return is the cell with
the smallest pencilmakrs stack.
*/
int sudoku_find_next_index(Sudoku* s) {
    //assume there exists no such cell
    Cell* best = NULL;
    //iterator cell
    Cell* c;
    //iterate over every cell
    for (int i = 0; i < 81; i++) {
        c = s->nodes[i];
        //ignore cells that are already filled in
        if (c->value != 0) continue;
        //if the best yet is non existent or the stack of the cell that
        //we currently observe is smaller than the current best
        if (best == NULL ||
           (stack_get_size(c->pencilmakrs) < stack_get_size(best->pencilmakrs))) {

            //make the current cell the current best
            best = c;
        }

        //if the stack if the current best is empty, since the size of the stack
        //will never be negative we can break out of the loop now and return the
        //current best as the best
        if (stack_is_empty(best->pencilmakrs)) break;
    }

    //if the best is non existent then return -1
    //otherwise return the index of the best
    return (best == NULL) ? NO_VALID_POS : best->index;
}

/*
This function calculates the pencilmakrs of each cell of the sudoku instance
*/
void sudoku_calculate_pencilmarks(Sudoku* s) {
    for (int i = 0; i < s->size; i++) {
        Cell* c = s->nodes[i];
        cell_calculate_pencilmarks(c, s->nodes);
    }
}

/*
This function returns false if two non-empty adjecent nodes have the same value
Otherwise true
*/
int sudoku_is_valid(Sudoku* s) {
    //for every cell
    for (int i = 0; i < s->size; i++) {
        Cell* c = s->nodes[i];
        //if the cell is empty ignore it
        if (c->value == 0) continue;

        //for every neighbor index of the cell
        for (int j = 0; j < 20; j++) {
            //get the neighbor
            Cell* other = s->nodes[c->neighbors[j]];

            //if the nodes have the same value
            if (c->value == other->value)
                //return false
                return 0;
        }
    }

    //we have iterated over all the cells and couldn't find any cell that
    //has the same value as one of its neighbors. So we can return true
    return 1;
}

/*
A sudoku is solved when it's valid and there exists no cell that can still be
filled in
*/
int sudoku_is_solved(Sudoku* s) {
    return sudoku_is_valid(s) && (sudoku_find_next_index(s) == NO_VALID_POS);
}
