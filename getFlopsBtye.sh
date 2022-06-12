#!/bin/bash

FILE="test.s"
FIND_STR="%vector.body"
flag=""
flops_num=0
byte_num=0
unit=4

cat $1 | while read line
do
if [[ -n $flag ]];then
temp=$(echo $line | grep -E 'z([0-9]|[1-2][0-9]|3[0-1]).')
#if [[ $line =~ "ld" ]] || [[ $line =~ "st" ]];then
if [[ $line =~ "ld" ]];then
if [[ $line =~ "ld1w" ]] || [[ $line =~ "ld1sw" ]];then
byte_num=`expr $byte_num + $unit`
elif [[ $line =~ "ld1b" ]] || [[ $line =~ "ld1sb" ]];then
byte_num=`expr $byte_num + $unit`
elif [[ $line =~ "ld1d" ]];then
byte_num=`expr $byte_num + $unit`
elif [[ $line =~ "ld1h" ]] || [[ $line =~ "ld1sh" ]];then
byte_num=`expr $byte_num + $unit`
fi
elif [[ $line =~ "st1w" ]];then
byte_num=`expr $byte_num + $unit`
elif [[ $line =~ "st1b" ]];then
byte_num=`expr $byte_num + $unit`
elif [[ $line =~ "st1h" ]];then
byte_num=`expr $byte_num + $unit`
elif [[ $line =~ "st1d" ]];then
byte_num=`expr $byte_num + $unit`
elif [[ $line =~ "st2" ]] || [[ $line =~ "st3" ]] || [[ $line =~ "st4" ]] || [[ $line =~ "stnt" ]];then
byte_num=`expr $byte_num + 0`
elif [[ -n $temp ]];then
flops_num=`expr $flops_num + 1`
fi

if [[ $line =~ $flag ]];then
echo -e "\033[31m==========$flag===========\033[0m"
echo "flops_num:"$flops_num
echo "byte_num:"$byte_num
flag=""
flops_num=0
byte_num=0
fi
elif [[ $line =~ $FIND_STR ]];then
   flag=$(echo $line | awk -F ': ' '{print $1}')
fi

done

