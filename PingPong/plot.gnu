set term png size 1000,1000

set output "bench.png"

set grid

set logscale x

set ylabel "Time"

set xlabel "Bytes (2^x)" 

plot "bench_B.txt" using 2:4 title "MPI blocking" with linespoint, "bench_NB.txt" using 2:4 title "MPI non-blocking" with linespoint