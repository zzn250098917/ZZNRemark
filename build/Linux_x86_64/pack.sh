#!/bin/sh
exe="ZZNRemark"
des="/home/jsbz/桌面/packApp"
deplist=$(ldd $exe | awk '{if (match($3,"/")){printf("%s "),$3 } }')
cp $deplist $des
