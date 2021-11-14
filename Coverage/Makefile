CC = mpicc
CFLAGS = -O3 -Wall
MPIFLAGS = -np 2

.PHONY: clean run

compil: covering

run: covering
ifndef BYTES
	@echo "BYTES is not defined"
else ifndef NBOP
	@echo "NBOP is not defined"
else
	mpirun ${MPIFLAGS} ./$< $(BYTES) $(NBOP)
endif

covering: covering.c
	${CC} ${CFLAGS} $^ -o $@

clean:
	rm -rf covering