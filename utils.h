#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "file.h"

#if !defined(UTILS_SUD)
#define UTILS_SUD

void array_print(int *a, int size);
float getTime();
char **create_sudoku_string_array_from_file(char *filename, int num_sudokus);
void free_sudoku_string_array(char **array, int size);
int strequals(char *s1, char *s2);

#endif // UTILS_SUD
