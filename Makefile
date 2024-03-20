# 定义编译器和编译选项
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# 源文件目录
SRCDIR = .

# 源文件列表
SRCS := $(wildcard $(SRCDIR)/*.c)

# 目标文件列表
OBJS := $(SRCS:.c=.o)

# 输出目标
TARGET = nstuck

# 默认目标
all: $(TARGET)

# 生成目标
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# 清理目标文件
clean:
	$(RM) $(OBJS) $(TARGET)