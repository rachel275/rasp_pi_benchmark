#!/bin/bash


for N in 16 32 64 128 256 512 768 1024
do
    echo "N = ${N}" >> oepngl_result
    ./opengl $N | grep 'time =' >> oepngl_result
    echo "" >> oepngl_result
done
