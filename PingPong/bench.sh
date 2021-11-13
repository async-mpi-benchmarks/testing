#!/bin/bash
rm -f tmp.txt
rm -f bench_B.txt
for ((i=1;i<=1048576;i=i*2));
	do 
		echo bytes $i on 1048576;
		for q in $(seq 1 100);
		do 
			mpirun --bind-to core --map-by core -np 2 ./B_pingpong $i | awk '/rdtsc:/ {print $2}' >> tmp.txt;
	done;
	awk -v i=$i '{x+=$1;next} END {printf"Bytes: %d mean: %d\n",i, x/NR}' tmp.txt >> bench_B.txt;
	rm tmp.txt;
done;

rm -f tmp.txt
rm -f bench_NB.txt
for ((i=1;i<=1048576;i=i*2));
	do 
		echo bytes $i on 1048576;
		for q in $(seq 1 100);
		do 
			mpirun --bind-to core --map-by core -np 2 ./NB_pingpong $i | awk '/rdtsc:/ {print $2}' >> tmp.txt;
	done;
	awk -v i=$i '{x+=$1;next} END {printf"Bytes: %d mean: %d\n",i, x/NR}' tmp.txt >> bench_NB.txt;
	rm tmp.txt;
done;
