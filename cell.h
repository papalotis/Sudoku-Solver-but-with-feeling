
#if !defined(CELL_H)
#define CELL_H

#include "stack.h"

typedef struct _Cell {

    int index;
    int x, y, box;
    int value;
    int* neighbors;
    stack* pencilmakrs;
} Cell;


Cell* create_cell();
void free_cell(Cell* c);
Cell* add_cell(int val, int index);
int cell_calculate_x(Cell* c);
int cell_calculate_y(Cell* c);
int cell_calculate_box(Cell* c);
int cell_is_empty(Cell* c);
void cell_calculate_neighbor_indeces(Cell* c);
void cell_calculate_pencilmarks(Cell* c, Cell** sud);
int cell_find_unique_pencilmarks(Cell* c, Cell** sud, int* indeces);
void cell_find_naked_pairs(Cell* c, Cell** sud, int* indeces);
int cell_calculate_error(Cell* c, Cell** sud);


#endif // CELL_H

