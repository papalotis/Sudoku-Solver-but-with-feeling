#include <stdio.h>
#include <time.h>
#include "sudoku.h"

void array_print(int* a, int size) {
    printf("Array %p: [", a);

    for (int i = 0; i < size; i++) {
        printf("%d", a[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}



int main(int argc, char *argv[]) {
    //the data that will be used to create the puzzle instance
    int sud[] = {
        2,6,0,0,0,0,0,0,0,
        0,1,0,0,9,0,0,7,0,
        0,0,7,0,0,0,1,0,2,
        0,0,0,8,5,0,0,0,1,
        9,0,8,0,0,0,2,0,3,
        1,0,0,0,4,9,0,0,0,
        4,0,9,0,0,0,5,0,0,
        0,5,0,0,7,0,0,2,0,
        0,0,0,0,0,0,0,9,6
    };

    int sud1[] = {8,0,0,0,0,0,0,0,0,
    0,0,3,6,0,0,0,0,0,
    0,7,0,0,9,0,2,0,0,
    0,5,0,0,0,7,0,0,0,
    0,0,0,0,4,5,7,0,0,
    0,0,0,1,0,0,0,3,0,
    0,0,1,0,0,0,0,6,8,
    0,0,8,5,0,0,0,1,0,
    0,9,0,0,0,0,4,0,0};

    //create a sudoku intance from the given array
    Sudoku* s = sudoku_create_from(sud1);
    //print the unsolved puzzle
    sudoku_print(s);
    //mark the time at which the function starts running
    float startTime = (float)clock()/CLOCKS_PER_SEC;
    //solve the puzzle
    int result = sudoku_solve(s);
    //mark the time at which the function ends
    float endTime = (float)clock()/CLOCKS_PER_SEC;
    //calculate the difference in time
    float elapsed = endTime - startTime;
    //print how much time went by
    printf("elapsed = %f\n", elapsed);
    if (result == SUDOKU_SOLVED) {
        //print the solved instance of the puzzle
        sudoku_print(s);
    }
    else {
        printf("This sudoku puzzle has no solution\n");
    }
    //free all the reserved memory
    free_sudoku(s);

}
