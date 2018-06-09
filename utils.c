#include "utils.h"

void array_print(int *a, int size)
{
    printf("Array %p: [", a);

    for (int i = 0; i < size; i++)
    {
        printf("%d", a[i]);
        if (i < size - 1)
            printf(", ");
    }
    printf("]\n");
}

float getTime()
{
    return (float)clock() / CLOCKS_PER_SEC;
}

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

void free_sudoku_string_array(char **array, int size)
{
    //free the string array
    for (int i = 0; i < size; i++)
    {
        free(array[i]);
    }
    free(array);
}

int strequals(char *s1, char *s2)
{
    return strcmp(s1, s2) == 0;
}

int compare_int(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int compare_float(const void *a, const void *b)
{
    return (*(float *)a - *(float *)b);
}

char *format_time_seconds(float timeSeconds, char *buff, int buff_size)
{
    memset(buff, 0, buff_size);

    char *minutes_suffix = " minutes";
    char *seconds_suffix = " seconds";
    char *commaspace = ", ";
    char num_buffer[20];

    float minutes = floor(timeSeconds / 60.0f);
    float seconds = timeSeconds - minutes * 60.0f;
    if (minutes > 0)
    {
        sprintf(num_buffer, "%.0f", minutes);
        strcat(buff, num_buffer);
        strcat(buff, minutes_suffix);
    }

    if (minutes > 0 && seconds > 0)
    {
        strcat(buff, commaspace);
    }

    if (seconds > 0)
    {
        sprintf(num_buffer, "%.2f", seconds);
        strcat(buff, num_buffer);
        strcat(buff, seconds_suffix);
    }

    return buff;
}