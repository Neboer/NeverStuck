#ifndef KILL_PROC_H
#define KILL_PROC_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define KILL_ERROR -1
#define KILL_SUCCESS 0
#define KILL_FORCE 1

typedef struct
{
    int exit_code;
    int status;
} ProcInfo;

ProcInfo *kill_process(int pid, int normal_exit_wait_sec);

#endif