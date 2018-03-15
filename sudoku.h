#include "cell.h"

#define SUDOKU_SOLVED 1
#define SUDOKU_NO_SOLUTUION -1
#define SUDOKU_UNDECIDED 0

typedef struct _Sudoku {
    Cell** nodes;
    int size;
    stack* indeces;
    int nextIndex;

    int** rows;
    int** columns;
    int** boxes;
} Sudoku;

Sudoku* create_sudoku();
void free_sudoku(Sudoku* s);
void sudoku_print(Sudoku* s);
Sudoku* sudoku_create_from(int* data);
int sudoku_solve_step(Sudoku* s);
int sudoku_solve(Sudoku* s);
int sudoku_find_next_index(Sudoku* s);
void sudoku_calculate_pencilmarks(Sudoku* s);
void sudoku_eliminate_pencilmakrs(Sudoku* s);
void sudoku_fill_rows_columns_boxes_arrays(Sudoku* s);
int sudoku_is_valid(Sudoku* s);
int sudoku_is_solved(Sudoku* s);
