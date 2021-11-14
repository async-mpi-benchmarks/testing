CC = mpicc
MPIC = mpirun
CFLAGS = -O3 -Wall -Wextra -g
MPIFLAGS = -np 2

.PHONY: clean exe

exe: mpi_test
	${MPIC} ${MPIFLAGS} ./$<

mpi_test: mpi_test.c
	${CC} ${CFLAGS} $^ -o $@ -lSDL2 

clean:
	rm -f mpi_test