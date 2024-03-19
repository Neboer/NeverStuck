#ifndef TIMEOUT_READ_H
#define TIMEOUT_READ_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>

#define READ_TIMEOUT -2
#define READ_ERROR -1
#define SELECT_ERROR -3
#define READ_EOF 0

ssize_t timeout_read(int fd, void *buf, size_t nbytes, int timeout_ms) ;

#endif

