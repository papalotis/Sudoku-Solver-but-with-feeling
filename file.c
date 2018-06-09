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
            if (c == ',')
            {
                while (1)
                {
                    c = fgetc(sudoku_file);
                    if (c == '\n')
                        break;
                }
            }
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

int get_number_of_lines_in_file(char *filename)
{
    //assume the file doesn't exist
    //a non existent file doesn't have any lines in it
    int lines = 0;

    FILE *fp = fopen(filename, "r");
    if (fp != NULL)
    {
        //if a file exists then it has at least one line
        lines = 1;
        char c;
        while ((c = fgetc(fp)) != EOF)
        {
            //for every newline we see increament the lines of the file
            lines += (c == '\n');
        }
    }
    fclose(fp);

    return lines;
}
