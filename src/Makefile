CC = mpicc
MPIC = mpirun
CFLAGS = -O3 -Wall -Wextra -g
MPIFLAGS = -np 2

.PHONY: clean exeB exe NB

run: exeB exeNB exe

exeB: mpi_testB
	${MPIC} ${MPIFLAGS} ./$<

mpi_testB: mpi_testB.c
	${CC} ${CFLAGS} $^ -o $@ -lSDL2 

exeNB: mpi_testNB
	${MPIC} ${MPIFLAGS} ./$<

mpi_testNB: mpi_testNB.c
	${CC} ${CFLAGS} $^ -o $@ -lSDL2 

exe: mpi_test
	${MPIC} ${MPIFLAGS} ./$<

mpi_test: mpi_test.c
	${CC} ${CFLAGS} $^ -o $@ -lSDL2 

clean:
	rm -f mpi_test mpi_testB mpi_testNB