#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <windows.h>
#include "map.h"
#include "main.h"

const char help_message[] = "\
Generate, check or solve sudoku\n\
\n\
Usage:\n\
    sudoku.exe function_code [input_file_name | --pipe]\n\
\n\
function_code should be an integer between 0 and 20.\n\
If function_code equals 0, the program will read from the input file and try to solve the sudoku the file contains.\n\
If function_code is between 1 and 20, the program will generate a sudoku with the given difficulty.\n\
And if --pipe is given as the second parameter, output will be formatted as valid input.\n\
If file name is not given, stdin / stdout will be used.\n\
\n\
Example:\n\
    sudoku.exe 0 hard.txt\n\
    sudoku.exe 2 level-2.txt\n\
";

const char invalid_cmdline[] = "Invalid command line parameters!\n\n";

const char ignored_cmdline[] = "Command line parameter ignored: \"%s\"\n";

const char invalid_file[] = "Error opening file \"%s\"\n\n";

const char invalid_file_content[] = "Malformed input file\n\n";

const char unexpected_end_of_file[] = "Unexpected end of file\n\n";

const char separator[] = "-----------------\n";

const char answer_hint[] = "\nAnswer:\n";

bool pipe_output = false;

// Entry point
int main(int argc, char *argv[])
{
    int func_code = parseCmdlineArgs(argc, argv, func_code);

    int *map = newMap();

    if (func_code == 0)
    {
        inputMapData(map);

        __int64 cycle_per_second, start_count, end_count;

        QueryPerformanceFrequency((LARGE_INTEGER *)&cycle_per_second);
        QueryPerformanceCounter((LARGE_INTEGER *)&start_count);

        int solutions = solve(map, false, true);

        QueryPerformanceCounter((LARGE_INTEGER *)&end_count);

        if (solutions)
        {
            outputMapData(map);

#ifdef DEBUG_MAP_GENERATION
            fprintf(stderr, "%d\n", solutions);
#endif
        }
        else
        {
            fputs(separator, stdout);
            fputs("-1\n", stdout);
            fputs(separator, stdout);
        }

        printf("Time used: %lldus\n", (end_count - start_count) * 1000000 / cycle_per_second);
    }
    else
    {
        int answer[81] = {};
        generate(map, answer, func_code);
        outputMapData(map);
        outputAnswer(answer);
    }

    free(map);

    return 0;
}

int parseCmdlineArgs(int &argc, char **argv, int &func_code)
{
    if (argc == 1)
    {
        puts(help_message);
        exit(0);
    }

    int return_code = sscanf(argv[1], "%d", &func_code);
    if (return_code == 0 || func_code < 0 || func_code > 20)
    {
        fprintf(stderr, invalid_cmdline);
        puts(help_message);
        exit(-1);
    }

    if (argc >= 3)
    {
        for (int i = 3; i < argc; i++)
        {
            fprintf(stderr, ignored_cmdline, argv[i]);
        }

        if (func_code == 0)
        {
            if (!freopen(argv[2], "r", stdin))
            {
                fprintf(stderr, invalid_file, argv[2]);
                exit(-1);
            }
        }
        else
        {
            if (!strcmp(argv[2], "--pipe"))
            {
                pipe_output = true;
            }
            else if (!freopen(argv[2], "w", stdout))
            {
                fprintf(stderr, invalid_file, argv[2]);
                exit(-1);
            }
        }
    }

    return func_code;
}

void inputMapData(int *map)
{
    for (int i = 0; i < 81; i++)
    {
        int return_code = scanf("%d", map + i);
        if (return_code <= 0 || map[i] < 0 || map[i] > 9)
        {
            if (return_code == EOF)
                fprintf(stderr, unexpected_end_of_file);
            else
                fprintf(stderr, invalid_file_content);

            exit(-1);
        }
    }
}

void outputMapData(int *map)
{
    if (!pipe_output)
        fputs(separator, stdout);

    for (int i = 0; i < 81; i++)
    {
        putchar('0' + map[i]);
        putchar(i % 9 == 8 ? '\n' : ' ');
    }

    if (!pipe_output)
        fputs(separator, stdout);
}

void outputAnswer(int *answer)
{
    FILE *f = pipe_output ? stderr : stdout;

    fputs(answer_hint, f);
    fputs(separator, f);

    for (int i = 0; i < 81; i++)
    {
        fputc('0' + answer[i], f);
        fputc(i % 9 == 8 ? '\n' : ' ', f);
    }

    fputs(separator, f);
}

int loadSudoku(char fileName[], int a[9][9]) {
    freopen(fileName, "r", stdin);
    inputMapData((int *)a);
}

int solveSudoku(int a[9][9]) {
    return solve((int *)a, false, true);
}

int genSudoku(int level, int a[9][9], int ans[9][9]) {
    generate((int *)a, (int *)ans, level);
    return 0;
}
