CC = mpicc
CFLAGS = -O3 -Wall
MPIFLAGS = -np 2

.PHONY: clean run

compil: main bench

run_bench: bench
	mpirun ${MPIFLAGS} ./$<

run: main
ifndef BYTES
	@echo "BYTES is not defined"
else ifndef NBOP
	@echo "NBOP is not defined"
else
	mpirun ${MPIFLAGS} ./$< $(BYTES) $(NBOP)
endif

bench: bench.c covering.o
	${CC} ${CFLAGS} $^ -o $@

main: main.c covering.o
	${CC} ${CFLAGS} $^ -o $@

clean:
	rm -rf covering