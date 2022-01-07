CC = mpicc
CFLAGS = -O3 -Wall
MPIFLAGS = -np 2

run: main

exe: main
	mpirun ${MPIFLAGS} ./$< $(BYTES)

main: main.c
	${CC} ${CFLAGS} $^ -o $@

clean:
	rm -f main