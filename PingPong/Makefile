CC = mpicc
CFLAGS = -O3 -Wall
MPIFLAGS = -np 2

.PHONY: clean run

run: NB_pingpong B_pingpong

exe_B: B_pingpong
ifndef BYTES
	@echo "BYTES is not defined"
else
	mpirun ${MPIFLAGS} ./$< $(BYTES)
endif

exe_NB: NB_pingpong
ifndef BYTES
	@echo "BYTES is not defined"
else
	mpirun ${MPIFLAGS} ./$< $(BYTES)
endif

NB_pingpong: NB_pingpong.c
	${CC} ${CFLAGS} $^ -o $@

B_pingpong: B_pingpong.c 
	${CC} ${CFLAGS} $^ -o $@

clean: 
	rm -f B_pingpong NB_pingpong
