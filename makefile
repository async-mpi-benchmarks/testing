CC = mpicc 
CFLAGS = -O2 -Wall

all: 1 2 3 3_mpi 3_omp

1 : 1.c
	${CC} ${CFLAGS} $^ -o $@ 
	
2 : 2.c
	${CC} ${CFLAGS} $^ -o $@ -lgmp
	
3 : 3.c
	${CC} ${CFLAGS} -march=native -mtune=native -funroll-loops -finline-functions -fpeel-loops -ftree-vectorize -ftree-loop-vectorize $^ -o $@ -lm -fopenmp
	
3_mpi : 3_mpi.c
	${CC} ${CFLAGS} -march=native -mtune=native -funroll-loops -finline-functions -fpeel-loops -ftree-vectorize -ftree-loop-vectorize $^ -o $@  -lm -fopenmp
	
	
3_omp : 3_omp.c
	${CC} ${CFLAGS} -march=native -mtune=native -funroll-loops -finline-functions -fpeel-loops -ftree-vectorize -ftree-loop-vectorize $^ -o $@ 	-lm  -fopenmp
			
	
clean :	
	rm 1 2 3 3_mpi 3_omp
	
run : 
	mpirun ./1 && mpirun ./2 &&  ./3 &&  ./3_mpi  && ./3_omp
	
