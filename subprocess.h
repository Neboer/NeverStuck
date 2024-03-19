#ifndef SUBPROCESS_H
#define SUBPROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct
{
    int pipefd_read;
    int pipefd_write;
    int pid;
} ExecInfo;

#define EXEC_SUCCESS 0
#define EXEC_FAIL 1

ExecInfo* fork_new_thread(char **arg_list);

#endif
