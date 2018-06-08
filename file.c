#include "file.h"

/**
 * 
 */
char **get_sudokus_from_file(char *fname, char **buff, int buff_len)
{
    FILE *sudoku_file = fopen(fname, "r");
    if (sudoku_file != NULL)
    {
        int counter = 0;
        char c;
        while ((c = fgetc(sudoku_file)) != EOF)
        {
            if (c == '\n')
                counter++;
            else
            {
                char str[2] = "\0";
                str[0] = c;
                strcat(buff[counter], str);
            }
            //if we have filled our buffer then break out of the reading loop
            if (counter == buff_len)
                break;
        }
    }
    fclose(sudoku_file);
    return buff;
}

int get_file_size(FILE *f)
{
    rewind(f);
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    rewind(f);
    return size;
}
