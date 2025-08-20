#!/bin/bash

echo 'enter n: '
read n
i=0
curr=1
while [ $((i)) -lt $((n)) ]; do
    echo $curr
    curr=$((curr+2))
    i=$((i+1))
done
