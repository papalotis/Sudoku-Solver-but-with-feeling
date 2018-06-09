#include "file.h"

/**
 * This function puts sudoku strings to a string array.
 * It receives a filename and a string array as arguments.
 * This function should be called on files with a specific 
 * format
 */
char **get_sudokus_from_file(char *fname, char **buff, int buff_len)
{
    //try to open the file
    FILE *sudoku_file = fopen(fname, "r");
    //if the file can be opend
    if (sudoku_file != NULL)
    {
        //count in which line we are
        int counter = 0;
        //the character we are about to read
        char c;
        //so long as we can read more files
        while ((c = fgetc(sudoku_file)) != EOF)
        {
            //if we read a comma that means we have read the entire sudoki
            //and now we are about to read the solution
            //for now we don't care about it so we read characters until
            //we get to the next line
            if (c == ',')
            {
                while (1)
                {
                    //read characters
                    c = fgetc(sudoku_file);
                    //up until a newline is read
                    if (c == '\n')
                        break;
                }
            }
            //if a newline is read
            if (c == '\n')
                //increament the counter so that the next sudoku string
                //goes to the next array position
                counter++;

            else //if we read a number
            {
                //allocate memory for a single number string
                char str[2] = "\0";
                str[0] = c;
                //and concatinate it to the string at that position in the array
                strcat(buff[counter], str);
            }
            //if we have filled the buffer then break out of the reading loop
            if (counter == buff_len)
                break;
        }
        //close the file
        fclose(sudoku_file);
    }

    //and return the string array
    return buff;
}

/**
 * This function returns the number of lines in a 
 * file, by counting how many newlines there are in the file
 */
int get_number_of_lines_in_file(char *filename)
{
    //assume the file doesn't exist
    //a non existent file doesn't have any lines in it
    int lines = 0;

    //attempt to open the file
    FILE *fp = fopen(filename, "r");
    //if we succeed
    if (fp != NULL)
    {
        //if a file exists then it has at least one line
        lines = 1;
        char c;
        //keep reading characters
        while ((c = fgetc(fp)) != EOF)
        {
            //for every newline we see increament the lines of the file
            lines += (c == '\n');
        }
        //close the file
        fclose(fp);
    }

    //return the number of lines
    return lines;
}
