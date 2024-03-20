#!/bin/bash

# 检查参数个数是否正确
if [ "$#" -ne 4 ]; then
    echo "Usage: $0 <timemin> <timemax> <str1> <str2>"
    exit 1
fi

# 提取参数
timemin=$1
timemax=$2
str1=$3
str2=$4

# 循环执行
while true; do
    # 生成随机等待时间
    random_time=$(shuf -i $timemin-$timemax -n 1)
    sleep $random_time
    
    # 输出str1
    echo $str1
    
    # 输出str2
    echo $str2
done
