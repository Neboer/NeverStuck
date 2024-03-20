#include "parse_option.h"
#include "stream_match.h"
#include "subprocess.h"
#include "timeout_read.h"
#include "kill_process.h"

#define BUFFER_SIZE 1024
#define PROG_SUCCESS 1
#define PROG_STUCK -1
#define PROG_ERROR -2

int wait_program_success(int fd_read, int timeout_ms, Matcher *matcher)
{
    char buffer[BUFFER_SIZE + 1];
    while (1)
    {
        ssize_t read_size = timeout_read(fd_read, buffer, BUFFER_SIZE, timeout_ms);
        if (read_size == READ_TIMEOUT)
        {
            // 程序输出超时！可能卡死了！
            return PROG_STUCK;
        }
        else if (read_size == SELECT_ERROR || read_size == READ_ERROR)
        {
            // 总之就是读错误
            return PROG_ERROR;
        }
        else if (read_size == READ_EOF)
        {
            // EOF了，发送方主动关闭了写端？
            return PROG_ERROR;
        }
        // 不论是读错误，还是写端被关闭，都不要再读了。
        else
        {
            // 程序必须持续输出子进程的输出。这很重要。在程序顺利启动后，守护者必须也继续转发程序输出。
            write(STDOUT_FILENO, buffer, read_size);
            // 赶紧组装个字符串。
            buffer[read_size] = '\0';
            // 先看看是不是启动了？
            MatchResult match_result = matcher_receive(matcher, buffer);
            if (match_result == MATCH)
            {
                // 程序启动成功！
                return PROG_SUCCESS;
            }
        }
        sleep(1); // 每秒读一次。
    }
}

int main(int argc, char **argv)
{
    Options *opts = parse_options(argc, argv);
    Matcher *matcher = matcher_init(opts->start_successful_output);

    ExecInfo *exec_thread = fork_new_thread(opts->arguments);

    close(exec_thread->pipefd_write);

    int wait_result = wait_program_success(exec_thread->pipefd_read, opts->max_stuck_time_sec * 1000, matcher);
    // 这里退出，说明matcher不需要继续使用了。回收其内存空间。
    matcher_destroy(matcher);
    if (wait_result == PROG_SUCCESS)
    {
        // 程序成功启动，开始转发程序的标准输出。此时的程序就是一个无情的转发机器。
        int status;
        ssize_t bytes_read;
        char buffer[BUFFER_SIZE];
        while ((bytes_read = read(exec_thread->pipefd_read, buffer, BUFFER_SIZE)) > 0)
        {
            write(STDOUT_FILENO, buffer, bytes_read);
        }
        // 如果程序读失败了……打印错误。
        if (bytes_read < 0)
        {
            perror("read");
        }
        // 保护子进程，直到子进程退出。
        waitpid(exec_thread->pid, &status, 0);
        return status; // 保证主进程的输出结果和子进程相同。
    }
    else if (wait_result == PROG_STUCK)
    {
        // 判断程序卡死，给程序20秒的时间退出
        ProcInfo *kill_result = kill_process(exec_thread->pid, 20 * 1000);
        if (kill_result->status != KILL_ERROR)
        {
            return kill_result->exit_code;
        }
        // 如果不能正确的杀死程序，那么只能等待程序自行退出了。
    }
    int status = 0;
    // 如果读取失败或EOF，阻塞的等待子进程退出即可。
    waitpid(exec_thread->pid, &status, 0);
    return status;
}