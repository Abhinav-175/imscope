#!/bin/bash

rate=5000
dt=$(bc -l <<< "1.0/$rate")

while true; do
    
    out=$(od -vAn -N8 -tu4 < /dev/urandom | awk '{printf "%.2f %.2f\n", 2 * ($1 / 4294967295.0) - 1, 2 * ($1 / 4294967295.0) - 1}')
    echo "$out"
    sleep "$dt"

done