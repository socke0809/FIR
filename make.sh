#! /bin/bash

gcc -std=c99 -o fir main.c -lm
./fir > output.dat
gnuplot -p -e "plot 'output.dat' using 1:3 title 'Output' with lines, 'output.dat' using 1:2 title 'Input' with lines"
