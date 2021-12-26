CC = mpicc 
CFLAGS = -O2 -Wall

all: 1 2

1 : 1.c
	${CC} ${CFLAGS} $^ -o $@ 
	
2 : 2.c
	${CC} ${CFLAGS} $^ -o $@ -lgmp
	
clean :	
	rm 1 2
	
run : 
	mpirun ./1 && mpirun ./2
	
