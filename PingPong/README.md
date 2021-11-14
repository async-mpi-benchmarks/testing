#Benchmark test for PingPong

##Usage:
	compilation:
	```bash
		make compil
	```
	 Exec blocking version:
	 ```bash
	 	make exe_B BYTES=[NB Bytes]
	 ```
	 Exec non-blovking version:
	 ```bash
	 	make exe_NB BYTES=[NB Bytes]
	 ```
	 Launch the benchmark:
	 ```bash
	 	./bench.sh
	 ```
	 Plot the result of the benchmark:
	 ```bash
	 	gnuplot plot.gnu
	 ```