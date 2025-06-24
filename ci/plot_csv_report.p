set datafile separator ","
set terminal png size 1280,1024
set output report_file.'.png'
set xlabel "Number of configs"
set x2label "Size of Files in bytes"
set ylabel "Average Load and Merge Time in ms"

set x2tics

plot \
 report_file using 1:3 axes x1y1 with linespoints title "Avg.Load & Merge Time -> Config Count", \
 "" using 2:3 axes x2y1 with linespoints title "Avg.Load & Merge Time -> Total Config Size"
