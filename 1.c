//this code is from internet 
// objecftive : discover mpi
// need 12 cores cpu (change world rank max)

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{
	MPI_Init(&argc,&argv);
	
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
 
	

	int number;
	if (world_rank == 0) {
		printf("The number of processes is %d\n" , world_size) ;	
	    for (int i = 1 ; i < world_size ; i++){
	    	    number = i;
	    	MPI_Send(&number, 1, MPI_INT, i, i, MPI_COMM_WORLD);
	    	}
	} else if (world_rank < world_size ) {
	    MPI_Recv(&number, 1, MPI_INT, 0, world_rank, MPI_COMM_WORLD,
		     MPI_STATUS_IGNORE);
	    printf("Process %d received value %d from process 0\n",
		   world_rank , number);
	}	
	MPI_Finalize();
}


