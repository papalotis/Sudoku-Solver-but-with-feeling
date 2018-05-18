#include <stdio.h>
#include <time.h>
#include "sudoku.h"

float getTime() {
    return (float)clock()/CLOCKS_PER_SEC;
}

int main(int argc, char *argv[]) {

    //the data that will be used to create the puzzle instance
    int sud[] = {
        0,0,0,3,0,7,4,0,0,
        9,0,0,0,0,4,0,0,8,
        3,7,0,0,0,0,0,6,0,
        8,2,0,9,0,0,6,0,0,
        0,0,1,2,0,0,9,0,4,
        0,4,0,0,3,8,0,5,0,
        2,0,8,6,9,0,7,0,0,
        0,9,0,0,0,0,0,0,0,
        7,5,0,0,0,0,0,0,6
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
    Sudoku* s = sudoku_create_from(sud);
    //print the unsolved puzzle
    sudoku_print(s);
    //mark the time at which the function starts running
    float startTime = getTime();
    //solve the puzzle
    int result = sudoku_solve(s);
    //mark the time at which the function ends
    float endTime = getTime();
    //calculate the difference in time
    float elapsed = endTime - startTime;
    //print how much time went by
    printf("\nSolved in %f seconds\n\n", elapsed);
    //if the puzzle had a solution
    if (result == SUDOKU_SOLVED) {
        //print the solved instance of the puzzle
        sudoku_print(s);
    }
    else {
        //otherwise, inform the user that that puzzle was unsolvable
        printf("This sudoku puzzle has no solution\n");
    }
    //free all the reserved memory
    free_sudoku(s);

}
