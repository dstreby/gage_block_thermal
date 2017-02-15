#!/usr/bin/env gnuplot

set title "Temperature Plot"
set xlabel "Sample Number"
set ylabel "Temperature (K)"
set yrange [260:400]
set y2range [0:100]
set ytics "10"
set y2tics "10"
#set logscale y

plot "sample.dat" using 1:2 axes x1y1 title "T_1" with lines, \
"sample.dat" using 1:3 axes x1y1 title "T_2" with lines, \
"sample.dat" using 1:4 axes x1y2 title "T_d" with lines

pause -1 "Press any key to continue..."
