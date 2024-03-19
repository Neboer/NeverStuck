#ifndef PARSE_OPTION_H
#define PARSE_OPTION_H

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    int max_stuck_time_sec;
    char *start_successful_output;
    char *arguments;
} Options;

Options* parse_options(int argc, char *argv[]);

#endif
