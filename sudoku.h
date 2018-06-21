
#if !defined(SUDOKU_H)
#define SUDOKU_H
#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "cell.h"

#define SUDOKU_SOLVED 1
#define SUDOKU_NO_SOLUTUION -1
#define SUDOKU_UNDECIDED 0

typedef struct _Sudoku
{
    Cell **nodes;
    int size;
    stack *indeces;
    stack *indeces_history;
    int nextIndex;

    int have_guessed;

    int print_history;

    int *empty_indeces;

    int *value_freq;

    int **rows;
    int **columns;
    int **boxes;

    int with_pencilmarks;
} Sudoku;

Sudoku *create_sudoku();
void free_sudoku(Sudoku *s);
int sudoku_set_with_pencilmarks(Sudoku *s, int dp);
int sudoku_set_print_history(Sudoku *s, int ph);
char *sudoku_to_string_simple(Sudoku *s, char *buff);
char *sudoku_to_string_fancy(Sudoku *s, char *buff);
void sudoku_print(Sudoku *s);
Sudoku *sudoku_create_from_int(int *data, int with_pencilmarks);
Sudoku *sudoku_create_from_char(char *data, int with_pencilmarks);
int sudoku_solve_step(Sudoku *s);
int sudoku_solve(Sudoku *s, int *result, int *steps);
int sudoku_do_pencilmarks(Sudoku *s);
int sudoku_fill_pencilmakrs_with_dumb_values(Sudoku *s);
int sudoku_num_possible_pencilmarks(Sudoku *s, int *empty_indeces);
void sudoku_calculate_value_frequency(Sudoku *s);
int sudoku_find_next_index(Sudoku *s);
void sudoku_calculate_pencilmarks(Sudoku *s);
void sudoku_find_hidden_pencilmakrs(Sudoku *s);
void sudoku_find_naked_pencilmarks_partners(Sudoku *s);
void sudoku_fill_rows_columns_boxes_arrays(Sudoku *s);
void sudoku_do_pointing_pairs(Sudoku *s);
void sudoku_do_box_pointing_pairs(Sudoku *s);
int sudoku_get_empty_indeces(Sudoku *s, int *buf);
int sudoku_is_valid(Sudoku *s);
int sudoku_is_solved(Sudoku *s);
int sudoku_calc_error(Sudoku *s);

#endif // SUDOKU_H
