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
 * 2.45 becomes -> 2 seconds 450 millis
 * 124.3 becomes -> 2 minutes, 4 seconds 300 millis
 * The arguments are the timevalue, and a buffer to put the string in
 */
char *format_time_seconds(float timeSeconds, char *buff, int buff_size)
{
    //reset the buffer
    memset(buff, 0, buff_size);

    //some constants
    char *s = "s";
    char *minutes_suffix = " minute";
    char *seconds_suffix = " second";
    char *millis_suffix = " millis";
    char *commaspace = ", ";
    char num_buffer[20];

    //get the number of minutes
    float minutes = floor(timeSeconds / 60.0f);
    //get the number of seconds that should be expressed as seconds
    float secondsNotOver60 = timeSeconds - minutes * 60.0f;
    float seconds = floor(secondsNotOver60);

    float millis = floor(1000 * (secondsNotOver60 - seconds));

    //if an amount of minutes should be printed
    if (minutes > 0)
    {
        //convert the number to a string
        sprintf(num_buffer, "%.0f", minutes);

        //and add it to the buffer
        strcat(buff, num_buffer);
        //with the appropriate suffix
        strcat(buff, minutes_suffix);
        if (minutes > 1)
            strcat(buff, s);
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
        sprintf(num_buffer, "%.0f", seconds);
        //add it to the buffer
        strcat(buff, num_buffer);
        //and add its suffix
        strcat(buff, seconds_suffix);
        if (seconds > 1)
            strcat(buff, s);
    }
    if (seconds > 0 && millis > 0)
    {
        //print a comma betweem them
        strcat(buff, commaspace);
    }
    if (millis > 0)
    {
        //convert the number to a string
        sprintf(num_buffer, "%.0f", millis);
        //add it to the buffer
        strcat(buff, num_buffer);
        //and add its suffix
        strcat(buff, millis_suffix);
    }

    //return the buffer
    return buff;
}

int get_bit(int value, int position)
{
    return (value >> position) & 0x1;
}

int set_bit(int *value, int position, short new_bit_value)
{

    int orig_value = *value;

    int mask_for_all_but_pos = ~(1 << position);

    //now the bit at the desired position is 0
    int new_value = orig_value & mask_for_all_but_pos;

    new_value |= (new_bit_value << position);
    *value = new_value;
}

int is_power_of_two(int val)
{
    return val && (!(val & (val - 1)));
}

int trailing_zeros(int val)
{
    return (val) ? __builtin_ctz(val) : -1;
}

int leading_zeros(int val)
{
    return (val) ? __builtin_clz(val) : -1;
}