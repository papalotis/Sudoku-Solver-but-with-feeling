#include "stack.h"

typedef struct _Cell {

    int index;

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
void cell_calculate_neighbor_indeces(Cell* c);
void cell_calculate_pencilmarks(Cell* c, Cell** sud);
