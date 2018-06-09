#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "file.h"

#define min(a, b) (a < b) ? a : b

#define floor_float(a) (float)((int)a)

#if !defined(UTILS_SUD)
#define UTILS_SUD

void array_print(int *a, int size);
float getTime();
char **create_sudoku_string_array_from_file(char *filename, int num_sudokus);
void free_sudoku_string_array(char **array, int size);
int strequals(char *s1, char *s2);
int compare_int(const void *a, const void *b);
int compare_float(const void *a, const void *b);
char *format_time_seconds(float timeSeconds, char *buff, int buff_size);

#endif // UTILS_SUD
