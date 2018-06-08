
#if !defined(FILE_H)
#define FILE_H


#include <stdio.h>
#include <string.h>
char** get_sudokus_from_file(char* fname, char** buff, int buff_len);
char* get_sudoku_by_index(char* sudokus, char* buff, int index);
int get_file_size(FILE* f);


#endif // FILE_H

