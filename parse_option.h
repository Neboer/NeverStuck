#ifndef PARSE_OPTION_H
#define PARSE_OPTION_H

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

struct Option
{
    int max_stuck_time_sec;
    char *start_successful_output;
    char *command;
    char *arguments;
};

struct Option parse_options(int argc, char *argv[]);

#endif
