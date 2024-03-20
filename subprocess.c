#include "subprocess.h"

int exec_and_output_to_fd(int *pipefd, char **arg_list)
{
    // 关闭管道读端
    close(pipefd[0]);

    // 将标准输出重定向到管道写端
    dup2(pipefd[1], STDOUT_FILENO);
    // dup2(pipefd[1], STDERR_FILENO);

    // 关闭不需要的文件描述符
    close(pipefd[1]);

    // 执行命令
    execvp(arg_list[0], arg_list);

    // 如果 execl 执行失败则输出错误信息
    perror("execvp");
    return EXIT_FAILURE;
}

// arg_list should be null terminated
ExecInfo* fork_new_thread(char **arg_list)
{
    int pipefd[2];
    pid_t pid;

    // 创建管道
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return NULL;
    }

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return NULL;
    }
    else if (pid == 0)
    { // 子进程
        int exec_result = exec_and_output_to_fd(pipefd, arg_list);
        // 子进程已经被exec代替。我们永远无法再控制子进程。
        if (exec_result == EXIT_FAILURE){
            return NULL;
        }
    }

    ExecInfo *exec_result = (ExecInfo *)malloc(sizeof(ExecInfo));
    exec_result->pid = pid;
    exec_result->pipefd_read = pipefd[0];
    exec_result->pipefd_write = pipefd[1];
    return exec_result;
}