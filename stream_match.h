#ifndef STREAM_MATCH_H
#define STREAM_MATCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATCH 1
#define NOT_MATCH 0

typedef int match_result;

typedef struct
{
    char *match_target;
    int target_length;

    char *buffer;
} Matcher;

Matcher *matcher_init(const char *match_target);

match_result matcher_receive(Matcher *matcher, const char *new_content);

void matcher_destroy(Matcher *matcher);

#endif
