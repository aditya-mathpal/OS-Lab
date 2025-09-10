#!/bin/bash

echo 'enter a, b, c from ax^2 + bx + c = 0: '
read a
read b
read c
t=$((b))*$((b))-4*$((a))*$((c))
case $t in
	0)
		echo 'one root'
		;;
	-*)
		echo 'no roots'
		;;esac
s=$(echo "scale=4; sqrt($((b))*$((b))-4*$((a))*$((c)))" | bc)
ans1=$(($((-$((b))+$((s))))/$((2*$((a))))))
ans2=$(($((-$((b))-$((s))))/$((2*$((a))))))
echo $ans1', '$ans2
