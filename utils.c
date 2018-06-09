#include "utils.h"

/**
 * This function prints an array to stdout
 */
void array_print(int *a, int size)
{
    //print the prefix
    printf("Array %p: [", a);

    //for every value
    for (int i = 0; i < size; i++)
    {
        //print the value
        printf("%d", a[i]);

        //if this is not the last value
        //print a comma for readability
        if (i < size - 1)
            printf(", ");
    }

    //print a closing bracket in the end
    printf("]\n");
}

/**
 * This function returns the system time in seconds
 */
float getTime()
{
    return (float)clock() / CLOCKS_PER_SEC;
}

/**
 * This function creates an array of sudoku strigns from a file
 */
char **create_sudoku_string_array_from_file(char *filename, int num_sudokus)
{

    //allocate space for the sudoku strings
    char **sud_str_array = (char **)malloc(num_sudokus * sizeof(char *));
    for (int i = 0; i < num_sudokus; i++)
    {
        //allocate memory for every sudoku string
        sud_str_array[i] = (char *)malloc(90 * sizeof(char));
        memset(sud_str_array[i], 0, 90);
    }

    return get_sudokus_from_file(filename, sud_str_array, num_sudokus);
}

/**
 * This function frees the sudoku string arrat from memory
 */
void free_sudoku_string_array(char **array, int size)
{
    //free every string
    for (int i = 0; i < size; i++)
    {
        free(array[i]);
    }
    //and then free the array itself
    free(array);
}

/**
 * This function returns true when two strings are equals
 */
int strequals(char *s1, char *s2)
{
    return strcmp(s1, s2) == 0;
}

/**
 * Order function for sorting an int array
 */
int compare_int(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

/**
 * Order function for sorting a float array
 */
int compare_float(const void *a, const void *b)
{
    return (*(float *)a - *(float *)b);
}

/**
 * This function receives a time value in seconds and converts it to
 * human readable string
 * 2.45 becomes -> 2.45 seconds
 * 124.3 becomes -> 2 minutes, 4.3 seconds
 * The arguments are the timevalue, and a buffer to put the string in
 */
char *format_time_seconds(float timeSeconds, char *buff, int buff_size)
{
    //reset the buffer
    memset(buff, 0, buff_size);

    //some constants
    char *minutes_suffix = " minutes";
    char *seconds_suffix = " seconds";
    char *commaspace = ", ";
    char num_buffer[20];

    //get the number of minutes
    float minutes = floor(timeSeconds / 60.0f);
    //get the number of seconds that should be expressed as seconds
    float seconds = timeSeconds - minutes * 60.0f;
    //if an amount of minutes should be printed
    if (minutes > 0)
    {
        //convert the number to a string
        sprintf(num_buffer, "%.0f", minutes);

        //and add it to the buffer
        strcat(buff, num_buffer);
        //with the appropriate suffix
        strcat(buff, minutes_suffix);
    }

    //if both minutes and seconds will be printed
    if (minutes > 0 && seconds > 0)
    {
        //print a comma betweem them
        strcat(buff, commaspace);
    }

    //if seconds are to be printed
    if (seconds > 0)
    {
        //convert the number to a string
        sprintf(num_buffer, "%.2f", seconds);
        //add it to the buffer
        strcat(buff, num_buffer);
        //and add its suffix
        strcat(buff, seconds_suffix);
    }

    //return the buffer
    return buff;
}