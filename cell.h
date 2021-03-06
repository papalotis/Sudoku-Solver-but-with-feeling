
#if !defined(CELL_H)
#define CELL_H

#include "pencilmarks_set.h"
#include <math.h>
#include <stdio.h>

typedef struct _Cell
{

    int index;
    int value;
    int *neighbors;
    int *row_neighbors;
    int *col_neighbors;
    int *box_neighbors;
    PSet *pencilmakrs;
} Cell;

Cell *create_cell();
void free_cell(Cell *c);
Cell *add_cell(int val, int index);
int cell_calculate_x(Cell *c);
int cell_calculate_y(Cell *c);
int cell_calculate_box(Cell *c);
int cell_is_empty(Cell *c);
int cell_retrieve_next_value_from_pencilmarks(Cell *c, int *value_freq);
void cell_calculate_neighbor_indeces(Cell *c);
void cell_calculate_pencilmarks(Cell *c, Cell **sud);
int cell_find_unique_pencilmarks(Cell *c, Cell **sud, int *indeces);
void cell_pointing_pair(Cell *c, Cell **sud, int *row_indeces, int *col_indeces, int *box_indeces);
void cell_find_naked_partners(Cell *c, Cell **sud, int *house_indeces);
void cell_get_neighbours_with_same_pencilmarks_in_house(Cell *c, Cell **sud, int *house_indeces, Cell **same_buffer,
                                                        int *same_num, Cell **comp_buffer, int *comp_num);
int cell_calculate_error(Cell *c, Cell **sud);

#endif // CELL_H
