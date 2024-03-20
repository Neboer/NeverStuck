#!/bin/bash



function handle_ctrlc()
{
    while true; do
        sleep 1
    done
}

# trapping the SIGINT signal
trap handle_ctrlc SIGTERM

# test_usage: ./nstuck -t 3 -o good -- ./test_program.sh 1 3 test good well
# 检查参数个数是否正确
if [ "$#" -lt 4 ]; then
    echo "Usage: $0 <timemin> <timemax> <strs>..."
    exit 1
fi

# 提取参数
timemin=$1
timemax=$2
shift 2
strs=("$@")

# 循环执行
while true; do
    # 生成随机等待时间
    random_time=$(shuf -i $timemin-$timemax -n 1)
    sleep $random_time
    >&2 echo "error"
    # 随机选择一个元素并输出
    size=${#strs[@]}
    index=$(($RANDOM % $size))
    echo ${strs[$index]}
done
