#include <string.h>
#include <stdio.h>
#include "sudoku.h"

#define INDEX_UNINITIALIZED -2
#define NO_VALID_POS -1

/*
A sudoku instance represents a sudoku puzzle at a specific state. It contains a
sudoku cell array (nodes) that represents all the cells that a sudoku is comprised of.
It has an indeces stack that is used to hold the backtracking path in which we
are. In has a size that represents the size of the sudoku puzzle.
nextIndex represents the index of the cell that is at the end of the backtracking
path. rows, columns and boxes are 2d arrays that contain the indeces of the cells
that belong to each row, column or box
*/

/*
This function creates an empty sudoku puzzle. It sets the size to 81 (as in a
9x9 grid). It allocates the necessary memory for the nodes array and the indeces
stack, and sets nextIndex to the appropriate value
*/
Sudoku *create_sudoku()
{
    //memory for sudoku object itself
    Sudoku *s = (Sudoku *)malloc(sizeof(Sudoku));
    s->size = 81;
    //memory for all the nodes
    s->nodes = (Cell **)malloc(sizeof(Cell *) * s->size);
    //empty stack
    s->indeces = create_stack();
    s->nextIndex = INDEX_UNINITIALIZED;

    //allocate memory for 3 2d arrays
    s->rows = (int **)malloc(sizeof(int *) * 9);
    s->columns = (int **)malloc(sizeof(int *) * 9);
    s->boxes = (int **)malloc(sizeof(int *) * 9);
    for (int i = 0; i < 9; i++)
    {
        s->rows[i] = (int *)malloc(sizeof(int) * 9);
        s->columns[i] = (int *)malloc(sizeof(int) * 9);
        s->boxes[i] = (int *)malloc(sizeof(int) * 9);
    }
    return s;
}

/*
This function frees a sudoku instance from memory. First it frees the memory used
by each cell that defines the sudoku puzzle. Then it frees the nodes array and
the indeces stack. It frees the 3 2d arrays and lastly it frees the memory used by itself
*/
void free_sudoku(Sudoku *s)
{
    for (int i = 0; i < s->size; i++)
    {
        free_cell(s->nodes[i]);
    }
    free(s->nodes);
    free_stack(s->indeces);

    for (int i = 0; i < 9; i++)
    {
        free(s->rows[i]);
        free(s->columns[i]);
        free(s->boxes[i]);
    }
    free(s->rows);
    free(s->columns);
    free(s->boxes);

    free(s);
}

/*This function converts a sudoku to a string. It receives a sudoku
and an optional string buffer as input and returns a string buffer
as output that contains the values of each cell. The sudoku string
has the following format, it is intended for data flow between programs
not for readability

000040009002010000500000073090000000004000100000507000001020000000003085000000000

*/
char *sudoku_to_string_simple(Sudoku *s, char *buff)
{
    //a simple string is 82 bytes long
    int str_len = 82;

    //if the caller didn't provide a buffer to write
    //we create one for ourselves.
    //WARNING: This needs to be freed by someone but not us
    if (buff == NULL)
    {
        buff = (char *)malloc(str_len * sizeof(char));
    }

    memset(buff, 0, str_len);

    //for every cell
    for (int i = 0; i < s->size; i++)
    {
        //get the value in it
        int val = s->nodes[i]->value;
        //convert it to a char
        char c = (char)(val) + '0';
        //put the char in the buffer
        buff[i] = c;
    }

    return buff;
}

/*
This function converts a sudoku to a string. It receives a sudoku
and an optional string buffer as input and returns a string buffer
as output that contains the values of each cell. The sudoku string
has the following format, it is intended for readability not for
data flow

_ _ _ | _ 4 _ | _ _ 9
_ _ 2 | _ 1 _ | _ _ _
5 _ _ | _ _ _ | _ 7 3
------+-------+------
_ 9 _ | _ _ _ | _ _ _
_ _ 4 | _ _ _ | 1 _ _
_ _ _ | 5 _ 7 | _ _ _
------+-------+------
_ _ 1 | _ 2 _ | _ _ _
_ _ _ | _ _ 3 | _ 8 5
_ _ _ | _ _ _ | _ _ _

*/
char *sudoku_to_string_fancy(Sudoku *s, char *buff)
{
    //a fancy string is 250 bytes long
    int str_len = 250;

    //if the caller didn't provide a buffer to write
    //we create one for ourselves.
    //WARNING: This needs to be freed by someone but not us
    if (buff == NULL)
    {
        buff = (char *)malloc(str_len * sizeof(char));
    }

    memset(buff, 0, str_len);

    //some constant strings that will be used
    char *dashes = "------+-------+------\n";
    char *pipe = "| ";
    char num[2];
    char *under = "_";
    char *space = " ";
    char *newline = "\n";

    //for every cell
    for (int i = 0; i < s->size; i++)
    {
        //the dashes and pipes that are printed are done so as to
        //differentiate between boxes

        //if we are at the beginnig of the forth or sevent line
        if (i == 27 || i == 54)
        {
            //print a line full of dashes
            strcat(buff, dashes);
        }

        //after every three cells printed in a row
        if (i % 9 == 3 || i % 9 == 6)
        {
            //print a pipe
            strcat(buff, pipe);
        }

        //get the value of a cell
        int val = s->nodes[i]->value;
        //for a value different from 0, we print that value
        if (val > 0)
        {
            //convert the value to a string
            sprintf(num, "%d", val);
            //add it to the buffer
            strcat(buff, num);
        }

        //otherwise we print an underscore to show that, that cell
        //needs to be filled in
        else
        {
            strcat(buff, under);
        }

        //after each cell if printed, print an empty character to make
        //the end result more readable
        strcat(buff, space);

        //after printing 9 cells print a newline so as to differentiate
        //between rows
        if (i % 9 == 8)
        {
            strcat(buff, newline);
        }
    }

    return buff;
}

/*
This function prints a sudoku instance at the state in which it is.
A cell is "empty" if it has the value 0 in it.
*/
void sudoku_print(Sudoku *s)
{
    //generate the string
    char *sud_str = sudoku_to_string_fancy(s, NULL);
    //print the string
    printf("%s\n", sud_str);
    //free it from memory
    free(sud_str);
}

/*
This function creates a sudoku puzzle and fills it in with the given data
*/
Sudoku *sudoku_create_from_int(int *data)
{
    //crete an empty sudoku instance
    Sudoku *s = create_sudoku();
    //for each value in the data
    for (int i = 0; i < s->size; i++)
    {
        //create a cell with the specific data and index and
        //add it to the nodes array
        s->nodes[i] = add_cell(data[i], i);
    }
    //
    sudoku_fill_rows_columns_boxes_arrays(s);

    //calculate the pencilmakrs of all the nodes of the new sudoku
    sudoku_calculate_pencilmarks(s);
    //initialize the nextIndex value
    s->nextIndex = sudoku_find_next_index(s);

    //return the created sudoku
    return s;
}

/*
This function creates a sudoku puzzle from a string by converting the
string to an integer array and then returning the result of the
sudoku_create_from_int function
*/
Sudoku *sudoku_create_from_char(char *data)
{
    //the int array
    int data_int[81];
    //for every char in the string
    for (int i = 0; i < 81; i++)
    {
        //get the char
        char c = data[i];
        //convert it to an int
        int n = (int)(c - '0');
        //set the data of the int array to that number
        data_int[i] = n;
    }
    //create the sudoku using the sudoku_create_from_int function
    Sudoku *s = sudoku_create_from_int(data_int);
    return s;
}

/*
This function runs one step of the solving algorithm
*/
int sudoku_solve_step(Sudoku *s)
{
    //the value that will be returned
    //assume we will keep on solving
    int r = SUDOKU_UNDECIDED;
    if (s->nextIndex == NO_VALID_POS)
    {
        r = SUDOKU_SOLVED;
    }
    else
    {

        //get the cell that we need to find the next value
        Cell *c = s->nodes[s->nextIndex];

        //get the next value from the pencilmakrs stack of that cell
        int val = stack_pop(c->pencilmakrs);
        //clean up the result of the stack (if it's negative that means the stack
        //was empty)
        c->value = val > 0 ? val : 0;

        //if there is no value so that the sudoku is still valid
        if (c->value == 0)
        {
            //pop the last value from the indeces stack
            //we move upwards in the backtracking tree
            if (!stack_is_empty(s->indeces))
            {
                s->nextIndex = stack_pop(s->indeces);
            }
            else
            {
                r = SUDOKU_NO_SOLUTUION;
            }
        }

        //if we found a value such that, that the sudoku is still valid
        else if (sudoku_is_valid(s))
        {
            //push the current index to the indeces array
            //we move downwards in the backtracking tree
            stack_push(s->indeces, s->nextIndex);

            //calculate the index of the next cell that we will try to fill in
            s->nextIndex = sudoku_find_next_index(s);

            //and calculate the pencilmakrs of the sudoku
            sudoku_calculate_pencilmarks(s);
            //find hidden singles
            sudoku_eliminate_pencilmakrs(s);
            //find naked pairs
            sudoku_find_naked_pencilmarks_pairs(s);
        }
    }
    return r;
}

/*
This function runs the sudoku solving algorithm up until the sudoku is solved
*/
int sudoku_solve(Sudoku *s)
{
    //assume that we will need to run at least one more step
    int r = SUDOKU_UNDECIDED;

    //in order to get to the solved instance of the puzzle we must
    //fill this many cells
    // int empty = sudoku_get_empty_indeces(s, NULL);

    //while the sudoku is not solved or it has NOT been determined that
    //there is no solution
    int counter = 0;
    while (r == SUDOKU_UNDECIDED)
    {
        //run a step of the solution algorithm
        //and get its decision
        r = sudoku_solve_step(s);
        counter++;
    }

    printf("\nSolved in %d steps\n", counter - 1);
    //return whether the sudoku was solved or has no solution
    return r;
}

/*
This function determines the index of the cell that we will run the
algorithm next on. The decision is made based on the size of the pencilmakrs
stack of each cell. The index of the cell that we want to return is the cell with
the smallest pencilmakrs stack.
*/
int sudoku_find_next_index(Sudoku *s)
{
    //assume there exists no such cell
    Cell *best = NULL;
    //iterator cell
    Cell *c;
    //iterate over every cell
    for (int i = 0; i < 81; i++)
    {
        c = s->nodes[i];
        //ignore cells that are already filled in
        if (c->value != 0)
            continue;
        //if the best yet is non existent or the stack of the cell that
        //we currently observe is smaller than the current best
        if (best == NULL ||
            (stack_get_size(c->pencilmakrs) < stack_get_size(best->pencilmakrs)))
        {

            //make the current cell the current best
            best = c;
        }

        //if the stack of the current best is empty, since the size of the stack
        //will never be negative we can break out of the loop now and return the
        //current best as the best
        if (stack_is_empty(best->pencilmakrs))
            break;
    }

    //if the best is non existent then return NO_VALID_POS
    //otherwise return the index of the best
    return (best == NULL) ? NO_VALID_POS : best->index;
}

/*
This function calculates the pencilmakrs of each cell of the sudoku instance
*/
void sudoku_calculate_pencilmarks(Sudoku *s)
{
    for (int i = 0; i < s->size; i++)
    {
        Cell *c = s->nodes[i];
        cell_calculate_pencilmarks(c, s->nodes);
    }
}

/*
This function finds hidden signles.
https://www.learn-sudoku.com/hidden-singles.html
*/
void sudoku_eliminate_pencilmakrs(Sudoku *s)
{
    //for every cell
    for (int i = 0; i < s->size; i++)
    {
        Cell *c = s->nodes[i];
        //get its x,y,box
        int cx = cell_calculate_x(c);
        int cy = cell_calculate_y(c);
        int cb = cell_calculate_box(c);

        //run the hidden single with the cell in its row
        cell_find_unique_pencilmarks(c, s->nodes, s->rows[cy]);
        //then the column
        cell_find_unique_pencilmarks(c, s->nodes, s->columns[cx]);
        //then the box
        cell_find_unique_pencilmarks(c, s->nodes, s->boxes[cb]);
    }
}

/*
This function finds naked pairs
https://www.learn-sudoku.com/naked-pairs.html
*/
void sudoku_find_naked_pencilmarks_pairs(Sudoku *s)
{
    //for every cell
    for (int i = 0; i < s->size; i++)
    {
        Cell *c = s->nodes[i];
        int cx = cell_calculate_x(c);
        int cy = cell_calculate_y(c);
        int cb = cell_calculate_box(c);

        cell_find_naked_pairs(c, s->nodes, s->rows[cy]);
        cell_find_naked_pairs(c, s->nodes, s->columns[cx]);
        cell_find_naked_pairs(c, s->nodes, s->boxes[cb]);
    }
}

void box_get_value_indeces(Sudoku *s, int *indeces, stack **app)
{
    //for every possible value
    for (int i = 1; i <= 9; i++)
    {
        //for every cell in the box
        for (int j = 0; j < 9; j++)
        {

            Cell *c = s->nodes[indeces[j]];
            //ignore filled in cells
            if (c->value != 0)
                continue;

            if (stack_contains(c->pencilmakrs, i))
            {
                stack_push(app[i], c->index);
            }
        }
    }
}

// void sudoku_apply_omission(Sudoku* s) {
//     stack* val_appearances[10];
//     //create 9 stacks
//     for (int i = 1; i <= 9; i++) {
//         val_appearances[i] = create_stack();
//     }
//     //for every box
//     for (int i = 0; i < 9; i++) {
//         for (int i = 1; i <= 9; i++) {
//             stack_clear(val_appearances[i]);
//         }
//         int* box_indeces = s->boxes[i];
//         box_get_value_indeces(s, box_indeces, val_appearances);
//
//     }
// }

/*
This function calculates the indeces of all the houses of a
sudoku puzzle. In theory this could be hard-coded but obviously this solution
is more elegant
*/
void sudoku_fill_rows_columns_boxes_arrays(Sudoku *s)
{
    //we need in total 27 counters for 27 houses
    //9 rows, 9 columns and 9 boxes

    int row_size = 9;

    int counters_rows[row_size];
    int counters_columns[row_size];
    int counters_boxes[row_size];
    //initialize all the counters with 0
    memset(counters_rows, 0, row_size * sizeof(int));
    memset(counters_columns, 0, row_size * sizeof(int));
    memset(counters_boxes, 0, row_size * sizeof(int));

    //for every cell
    for (int i = 0; i < s->size; i++)
    {
        Cell *c = s->nodes[i];
        //x gives us which column
        int cx = cell_calculate_x(c);
        //y which row
        int cy = cell_calculate_y(c);
        //b which box
        int cb = cell_calculate_box(c);

        //put that index in the appropriate column array and increament the counter
        s->columns[cx][counters_columns[cx]++] = i;
        //same for row
        s->rows[cy][counters_rows[cy]++] = i;
        //and box
        s->boxes[cb][counters_boxes[cb]++] = i;
    }
}

/**
 * This function receives a buffer as input and fills it
 * with the indeces of the cells of the sudoku that are still empty.
 * The function also returns how many cells should still be filled
 */
int sudoku_get_empty_indeces(Sudoku *s, int *buf)
{
    //if a buffer was given fill it with -1
    if (buf)
    {
        memset(buf, -1, 81 * sizeof(int));
    }

    int counter = 0;
    //for every cell
    for (int i = 0; i < s->size; i++)
    {
        Cell *c = s->nodes[i];
        //if the cell is empty
        if (cell_is_empty(c))
        {
            //add the index to the buffer (if it was given)
            if (buf)
            {
                buf[counter] = i;
            }
            //increament how many empty cells there are
            counter++;
        }
    }
    //return the number of empty cells
    return counter;
}

/*
This function returns false if two non-empty adjecent nodes have the same value
Otherwise true
*/
int sudoku_is_valid(Sudoku *s)
{
    //for every cell
    for (int i = 0; i < s->size; i++)
    {
        Cell *c = s->nodes[i];
        //if the cell is empty ignore it
        if (c->value == 0)
            continue;

        //for every neighbor index of the cell
        for (int j = 0; j < 20; j++)
        {
            //get the neighbor
            Cell *other = s->nodes[c->neighbors[j]];

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
int sudoku_is_solved(Sudoku *s)
{
    return sudoku_is_valid(s) && (sudoku_find_next_index(s) == NO_VALID_POS);
}
