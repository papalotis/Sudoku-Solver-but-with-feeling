#include "file.h"

/**
 * This function is broken. DO NOT USE IT FOR NOW
 */
char** get_sudokus_from_file(char* fname, char** buff, int buff_len) {
    memset(buff, 0, buff_len);
    printf("before openning the file\n");
    FILE* sudoku_file = fopen (fname,"r");
    printf("hi this is some text\n");
    if (sudoku_file != NULL) {
        int counter = 0;
        int len = 81;
        printf("before the loop\n");
        while (fgets(buff[counter], len, sudoku_file)) {
            // printf("%s\n", buff[counter]);
            printf("%d\n", counter);
            counter++;

        }
    }

    printf("end\n");

    fclose(sudoku_file);
    return buff;
}


char* get_sudoku_by_index(char* sudokus, char* buff, int index) {
    int factor = 83;
    int sud_len = 82;
    memset(buff, 0, sud_len);

    int offset = index * factor;
    printf("offset = %d\n", offset);

    int i = 0;
    while (sudokus[offset + i] != '\n') {
        buff[i] = sudokus[offset + i];
        i++;
    }

    buff[sud_len] = '\0';
    // printf("buff[sud_len - 1] == '\\0' = %d\n", buff[sud_len - 1] == '\0');
    // printf("strlen(buff) = %d\n", strlen(buff));
    printf("%s", buff);

    return buff;
}

int get_file_size(FILE* f) {
    rewind(f);
    fseek (f , 0 , SEEK_END);
    int size = ftell(f);
    rewind(f);
    return size;
}
