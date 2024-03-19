#ifndef TIMEOUT_READ_H
#define TIMEOUT_READ_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>

#define READ_TIMEOUT -2

ssize_t read_timeout(int fd, void *buf, size_t nbytes, int timeout_ms) ;

#endif

