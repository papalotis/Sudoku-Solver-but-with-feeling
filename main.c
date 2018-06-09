#include <stdio.h>
#include <time.h>
#include <limits.h>
#include "sudoku.h"
#include "file.h"
#include "utils.h"

/*GLOBAL VARS*/
//the name of the file we want to open
char filename[40] = "data/hardest_sudokus.txt";

//whether we want to print the result of each sudoku
int print = 1;

//how many sudokus we will solve
int num_sudokus = INT_MAX;

//whether we are going to use pencilmarks
//for solving this puzzle
int with_pencilmarks = 1;

#pragma region arguments
/**
 * This function handles the command line arguments
 * and sets the global variables appropriatelly
 */
void handle_args(int argc, char *argv[])
{
    //the type of arguments that can be given
    char *file_flag = "-f";
    char *no_print = "--noprint";
    char *no_print_abr = "-np";
    char *num_suds = "-n";
    char *pencilmarks = "-pencilmarks";
    char *pencilmarks_abr = "-p";

    //the zero'th argument is the program
    //itself, so start from the first arguemnt
    for (int i = 1; i < argc; i++)
    {
        //get the argument
        char *arg = argv[i];
        //if the name of the file is specified
        if (strequals(arg, file_flag))
        {
            //we need to look at the next argument
            //because that is the name of the file
            i++;
            //get the filename
            char *fname = argv[i];
            //put the argument in the global filename variable
            memset(filename, 0, 40);
            strcat(filename, fname);
        }

        //if the user specified that no printing should take place
        if (strequals(arg, no_print) || strequals(arg, no_print_abr))
        {
            //then set the global variable to false
            print = 0;
        }

        //if the user specified how many sudokus should be solved
        if (strequals(arg, num_suds))
        {
            //we need to look at the next arguments
            //because that will tell us the actual number
            i++;
            //get the value and convert it to a number
            //and store it in the global variable
            num_sudokus = atoi(argv[i]);
        }

        if (strequals(arg, pencilmarks) || strequals(arg, pencilmarks_abr))
        {
            //we need to look at the next arguments
            //because that will tell us the actual number
            i++;
            //get the value and convert it to a number
            //and store it in the global variable
            with_pencilmarks = atoi(argv[i]);
        }
    }
}

#pragma endregion

/**
 * The entry point of the program
 */
int main(int argc, char *argv[])
{
    //handle the arguments and set the global variables
    handle_args(argc, argv);

    //the maximum amount of sudokus we want to read from a file
    //is the smallest number between the number of sudokus in the file
    //and the number specified by the user
    num_sudokus = min(get_number_of_lines_in_file(filename), num_sudokus);
    //create the string array and load the sudokus
    char **sud_str_array = create_sudoku_string_array_from_file(filename, num_sudokus);

    //the number of sudokus that we solved
    int solved = 0;
    //and those we haven't solved
    int not_solved = 0;

    //Arrays to hold information for each attempt
    //The number of steps it took to get to the solution
    int *stepsForEach = (int *)malloc(num_sudokus * sizeof(int));
    //The number of cells that were empty at the start
    int *emptyAtStartForEach = (int *)malloc(num_sudokus * sizeof(int));
    //The amount of time needed to find the solution
    float *timeForEach = (float *)malloc(num_sudokus * sizeof(float));

    //set the arrays to start with zeros
    memset(stepsForEach, 0, num_sudokus);
    memset(emptyAtStartForEach, 0, num_sudokus);
    memset(timeForEach, 0, num_sudokus);

    float avgTime = 0;
    float avgSteps = 0;

    //get the time where we start solvin
    float timeWeStartGoingThoughThePuzzles = getTime();

    //for every sudoku string that we read
    for (int i = 0; i < num_sudokus; i++)
    {
        //get the sudoku string
        char *sud_to_solve = sud_str_array[i];

        //create a sudoku intance from the given array
        Sudoku *s = sudoku_create_from_char(sud_to_solve, with_pencilmarks);

        if (print) //print the unsolved puzzle if the user wants us to
            sudoku_print(s);

        //get how many cells are empty before we start solving for this sudoku
        emptyAtStartForEach[i] = sudoku_get_empty_indeces(s, NULL);

        //these ints are given to the solving function in order to
        //store what the result was and how many steps it took
        //us to get there
        int result, steps;

        //mark the time at which the function starts running
        float thisStartTime = getTime();

        //attempt solve the puzzle
        sudoku_solve(s, &result, &steps);

        //mark the time at which the function ends
        float thisEndTime = getTime();

        //calculate the difference in time
        float elapsed = thisEndTime - thisStartTime;

        //put the steps needed and the time used in the appropiate
        //arrays
        stepsForEach[i] = steps;
        timeForEach[i] = elapsed;

        avgTime += elapsed;
        avgSteps += steps;

        //print how much time went by and how many steps it took us
        //only if the user wants us to
        if (print)
        {
            printf("Solved in %f seconds\n", elapsed);
            printf("Solved in %d steps\n\n", steps);
        }
        //if the puzzle had a solution
        if (result == SUDOKU_SOLVED)
        {

            if (print) //print the solved instance of the puzzle if the user wants us to
                sudoku_print(s);
            //and increase the number of sudokus we have successfully solved by 1;
            solved++;
        }
        else //otherwise if the sudoku hasn't been solved
        {
            //inform the user that that puzzle has no solution
            //but only if he wants us to
            if (print)
                printf("This sudoku puzzle has no solution\n");

            //and increase the number of sudokus we couldn't solve by 1
            not_solved++;
        }

        //free the sudoku we created
        free_sudoku(s);
    }

    avgSteps /= num_sudokus;
    avgTime /= num_sudokus;

    //get the time after we have solved all the puzzles
    float timeWeFinishGoingThoughThePuzzle = getTime();
    //and calculate how much time elapsed since the start

    float totalTime = timeWeFinishGoingThoughThePuzzle - timeWeStartGoingThoughThePuzzles;

    //sort the three metric arrays, so that we can get the median values
    qsort(stepsForEach, num_sudokus, sizeof(int), compare_int);
    qsort(emptyAtStartForEach, num_sudokus, sizeof(int), compare_int);
    qsort(timeForEach, num_sudokus, sizeof(int), compare_float);

    //allocate some memory so that we can print the time easier
    char timeBuff[40];

    //inform the user about the final results
    printf("Attempted to solve %d sudoku%s\n", num_sudokus, (num_sudokus != 1) ? "s" : "");
    printf("For %d of them a solution was found\n", solved);
    printf("%.2f%% of the sudokus were solved\n", 100 * (float)solved / (num_sudokus));
    printf("Median steps until solution %d\n", stepsForEach[num_sudokus / 2]);
    printf("Average steps for solution %.0f\n", avgSteps);
    printf("Median empty cells at start %d\n", emptyAtStartForEach[num_sudokus / 2]);
    printf("Median time for solution %.4fs\n", timeForEach[num_sudokus / 2]);
    printf("Average time for solution %.4fs\n", avgTime);

    printf("Total time: %s\n", format_time_seconds(totalTime, timeBuff, 40));

    //free the string array
    free_sudoku_string_array(sud_str_array, num_sudokus);
    //and the three metric arrays
    free(stepsForEach);
    free(emptyAtStartForEach);
    free(timeForEach);
}
