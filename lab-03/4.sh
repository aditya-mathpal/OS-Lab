#!/bin/bash

echo 'enter n: '
read n
res=1
while [ $((n)) -gt 0 ]; do
    res=$((res*n))
    n=$((n-1))
done
echo $res
