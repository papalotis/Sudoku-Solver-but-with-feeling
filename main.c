#include <stdio.h>
#include <time.h>
#include "sudoku.h"
#include "file.h"
#include "utils.h"

//the data that will be used to create the puzzle instance
int sud[] = {
    0, 0, 0, 0, 4, 0, 0, 0, 9,
    0, 0, 2, 0, 1, 0, 0, 0, 0,
    5, 0, 0, 0, 0, 0, 0, 7, 3,
    0, 9, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 5, 0, 7, 0, 0, 0,
    0, 0, 1, 0, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 3, 0, 8, 5,
    0, 0, 0, 0, 0, 0, 0, 0, 0};

int sud1[] = {
    8, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 3, 6, 0, 0, 0, 0, 0,
    0, 7, 0, 0, 9, 0, 2, 0, 0,
    0, 5, 0, 0, 0, 7, 0, 0, 0,
    0, 0, 0, 0, 4, 5, 7, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 3, 0,
    0, 0, 1, 0, 0, 0, 0, 6, 8,
    0, 0, 8, 5, 0, 0, 0, 1, 0,
    0, 9, 0, 0, 0, 0, 4, 0, 0};
char *sud2 = "000000008003000400090020060000079000000061200060502070008000500010000020405000003";
char *sud3 = "120400300300010050006000100700090000040603000003002000500080700007000005000000098";
char *sud4 = "000000003090106740704300690500012004000000000400860007043001802067908030800000000";

char filename[40] = "hardest_sudokus.txt";
int print = 1;

void handle_args(int argc, char *argv[])
{

    char *file_flag = "-f";
    char *no_print = "--noprint";
    char *no_print_abr = "-np";

    for (int i = 1; i < argc; i++)
    {
        char *arg = argv[i];
        //if we will read from a file
        if (strequals(arg, file_flag))
        {
            //we need to look at the next argument
            i++;
            //get the filename
            char *fname = argv[i];
            //put the argument in the global filename variable
            memset(filename, 0, 40);
            strcat(filename, fname);
        }
        if (strequals(arg, no_print) || strequals(arg, no_print_abr))
        {
            print = 0;
        }
    }
}

int main(int argc, char *argv[])
{

    handle_args(argc, argv);

    //the maximum amount of sudokus we want to read from a file
    int num_sudokus = get_number_of_lines_in_file(filename);
    char **sud_str_array = create_sudoku_string_array_from_file(filename, num_sudokus);

    int solved = 0;
    int not_solved = 0;

    float timeWeStartGoingThoughThePuzzle = getTime();

    for (int i = 0; i < num_sudokus; i++)
    {
        char *sud_to_solve = sud_str_array[i];
        if (strlen(sud_to_solve) == 0)
            break;

        //create a sudoku intance from the given array
        Sudoku *s = sudoku_create_from_char(sud_to_solve);
        //print the unsolved puzzle
        sudoku_print(s);
        //mark the time at which the function starts running
        float thisStartTime = getTime();

        int result, steps;
        //attempt solve the puzzle
        sudoku_solve(s, &result, &steps);
        //mark the time at which the function ends
        float thisEndTime = getTime();
        //calculate the difference in time
        float elapsed = thisEndTime - thisStartTime;
        //print how much time went by
        printf("Solved in %f seconds\n", elapsed);
        printf("Solved in %d steps\n\n", steps);
        //if the puzzle had a solution
        if (result == SUDOKU_SOLVED)
        {
            //print the solved instance of the puzzle
            sudoku_print(s);
        }
        else
        {
            //otherwise, inform the user that that puzzle was unsolvable
            printf("This sudoku puzzle has no solution\n");
        }

        //free the sudoku we created
        free_sudoku(s);
    }

    float timeWeFinishGoingThoughThePuzzle = getTime();
    float totalTime = timeWeFinishGoingThoughThePuzzle - timeWeStartGoingThoughThePuzzle;
    if (print || 1)
    {
        printf("Attempted to solve %d sudokus\n", solved + not_solved);
        printf("For %d of them a solution was found\n", solved);
        printf("%.2f%% of the sudokus were solved\n", 100 * (float)solved / (solved + not_solved));
        printf("Total time: %.2f seconds\n", totalTime);
    }

    free_sudoku_string_array(sud_str_array, num_sudokus);
}
