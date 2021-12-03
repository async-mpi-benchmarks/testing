exec:MPI_NBC
    mpirun -np 2 ./MPI_NBC
MPI_NBC:MPI_NBC.c
    mpicc MPI_NBC.c -o MPI_NBC
