#!/bin/bash
# 一个简单的脚本，用于把windows路由表子网掩码格式转换成linux路由表子网掩码格式
# 例如，把255.255.255.0转换成/24
# 原理是把每个八位二进制数转换成十进制数，然后计算其中有多少个1，最后把所有的1的个数相加
# 参考资料：[1] [2]

# 定义一个函数，用于把十进制数转换成二进制数，并计算其中有多少个1
dec2bin() {
  local dec=$1 # 输入的十进制数
  local bin="" # 输出的二进制数
  local count=0 # 1的个数
  # 用除2取余的方法，从低位到高位得到二进制数
  while [ $dec -gt 0 ]; do
    local rem=$((dec % 2)) # 取余数
    bin="$rem$bin" # 拼接二进制数
    dec=$((dec / 2)) # 除以2
    # 如果余数是1，就把1的个数加1
    if [ $rem -eq 1 ]; then
      count=$((count + 1))
    fi
  done
  # 返回1的个数
  echo $count
}

# 定义一个函数，用于把windows路由表子网掩码格式转换成linux路由表子网掩码格式
win2lin() {
  local win_mask=$1 # 输入的windows路由表子网掩码格式，例如255.255.255.0
  local lin_mask=0 # 输出的linux路由表子网掩码格式，例如/24
  # 用点号分割输入的子网掩码，得到四个十进制数
  local IFS="."
  read -r -a dec_array <<< "$win_mask"
  # 对每个十进制数，调用dec2bin函数，得到其中1的个数，并累加到lin_mask中
  for dec in "${dec_array[@]}"; do
    lin_mask=$((lin_mask + $(dec2bin $dec)))
  done
  # 在lin_mask前面加上斜杠，得到linux路由表子网掩码格式
  lin_mask="/$lin_mask"
  # 返回linux路由表子网掩码格式
  echo $lin_mask
}

# 创建一个空文件，用于存储结果
touch linuxroute.txt

# 从route.txt中按行读取windows路由表子网掩码格式数据
while read -r line; do
  # 调用win2lin函数，把每行的数据转换成linux路由表子网掩码格式，并输出到linuxroute.txt中
  echo "$line -> $(win2lin $line)" >> linuxroute.txt
done < route.txt
