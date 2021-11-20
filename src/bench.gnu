set term png size 700,700

set output "result.png"

set grid

set ylabel "Latency in cycles"

set xlabel "number of Bytes"

plot "result0.dat" using 10:8 t "isend version" w lp, "result1.dat" using 6:2 t "send version" w lp