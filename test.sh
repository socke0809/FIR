#! /bin/bash

./main.elf > output.dat
gnuplot -p -e "plot 'output.dat' using 1:2 title 'Input' with lines, 'output.dat' using 1:3 title 'Output' with lines, 'output.dat' using 1:4 title 'RMS' with lines"
