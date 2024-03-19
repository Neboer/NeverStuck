#include "timeout_read.h"

ssize_t timeout_read(int fd, void *buf, size_t nbytes, int timeout_ms) {
    fd_set fds;
    struct timeval tv;
    ssize_t ret;

    // 初始化文件描述符集合
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    // 设置超时时间
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    // 调用select函数等待文件描述符就绪
    ret = select(fd + 1, &fds, NULL, NULL, &tv);

    // 如果select返回0，表示超时
    if (ret == 0) {
        errno = ETIMEDOUT;
        return READ_TIMEOUT;
    }

    // 如果select返回-1，表示发生错误
    if (ret == -1) {
        perror("select");
        return SELECT_ERROR;
    }

    // 如果文件描述符在超时时间内变为可读状态，则调用read函数读取数据
    ret = read(fd, buf, nbytes);
    if (ret == -1) {
        perror("read");
        return READ_ERROR;
    }
    return ret;
}