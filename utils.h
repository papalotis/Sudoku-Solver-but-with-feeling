#if !defined(UTILS_SUD)
#define UTILS_SUD

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "file.h"

#define min(a, b) (a < b) ? a : b

#define bit_is_set(b, n) b &(1 << n) != 0

#define floor_float(a) (float)((int)a)

void array_print(int *a, int size);
float getTime();
char **create_sudoku_string_array_from_file(char *filename, int num_sudokus);
void free_sudoku_string_array(char **array, int size);
int strequals(char *s1, char *s2);
int compare_int(const void *a, const void *b);
int compare_float(const void *a, const void *b);
char *format_time_seconds(float timeSeconds, char *buff, int buff_size);
int get_bit(int value, int position);
int set_bit(int *value, int position, short new_bit_value);
int is_power_of_two(int val);
int trailing_zeros(int val);

#endif // UTILS_SUD
