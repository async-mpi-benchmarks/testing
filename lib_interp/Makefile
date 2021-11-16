all: main libimpi.so

main: main.c libbar.so liblol.so
	gcc main.c -o $@ -llol -lbar  -L. -Wl,-rpath=$(PWD)

libbar.so: lib.c
	gcc -fpic -shared $^ -o $@

liblol.so: a.c
	gcc -fpic -shared $^ -o $@


libimpi.so: impi.c
	mpicc -fpic -shared $^ -o $@
