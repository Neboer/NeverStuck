#include "kill_process.h"

ProcInfo *kill_proc_grp_by_pid(int pid, int normal_exit_wait_sec)
{
    ProcInfo *kill_result = malloc(sizeof(ProcInfo));
    int pgid = getpgid(pid);
    // 向进程发送终止信号
    if (killpg(pgid, SIGTERM) == -1)
    {
        perror("killpg");
        kill_result->status = KILL_ERROR;
        return kill_result;
    }

    // 等待进程退出
    int remaining_time = normal_exit_wait_sec;
    while (remaining_time > 0)
    {
        // 不要阻塞的等待。
        pid_t result = waitpid(pid, &(kill_result->exit_code), WNOHANG);
        if (result > 0)
        {
            kill_result->status = KILL_SUCCESS;
            return kill_result;
        }
        else if (result == 0)
        {
            // 进程尚未退出，等待1秒
            sleep(1);
            remaining_time -= 1;
        }
        else
        {
            perror("waitpid");
            kill_result->status = KILL_ERROR;
            return kill_result;
        }
    }

    // 等待超时，强行杀死进程
    if (killpg(pgid, SIGTERM) == -1)
    {
        perror("killpg");
        kill_result->status = KILL_ERROR;
        return kill_result;
    }
    kill_result->status = KILL_FORCE;
    return kill_result;
}