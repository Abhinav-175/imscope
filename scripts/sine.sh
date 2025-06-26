#!/bin/bash

rate=5000
freq=5
amp=1.0

dt=$(bc -l <<< "1.0/$rate")
omega=$(bc -l <<< "2 * 4 * a(1) * $freq")

T=0.0
while true; do
    out=$(bc -l <<< "$amp * s($omega * $T)" | awk '{printf "%.2f\n", $1}')
    echo "$out"
    T=$(bc -l <<< "$T + $dt" )
    sleep "$dt"
done