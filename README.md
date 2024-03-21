# 启动守护

NeverStuck是一个启动守护工具。你可以让它启动一个程序，并告诉他程序输出什么才算启动成功。

NeverStuck会立即启动程序。在它判断程序启动成功之前，如果程序超过您指定的超时时间仍然没有输出任何新内容，NeverStuck将会认为程序卡死，它会强行杀死程序所在的进程组。

## 使用方法

```shell
./nstuck --max-stuck-time <seconds> --start-successful-output <message> -- <command> [arguments...]
```

- `--max-stuck-time <seconds>`/`-t`: 指定程序启动的最长超时时间（以秒为单位）。如果程序在超时时间内没有输出任何新内容，则被认为卡死。
- `--start-successful-output <message>`/`-o`: 指定程序启动成功的输出消息。当程序输出该消息时，被认为启动成功。
- `<command> [arguments...]`: 要启动的程序及其参数。

## 示例

以下是使用NeverStuck的示例命令：

```shell
./nstuck -t 2 -o hello -- ./test_program.sh 0 4 foo bar test hello
```

## 构建与开发

程序使用Linux POSIX C标准编写，使用`make`在程序文件夹下启动构建，执行`./nstuck`启动程序。