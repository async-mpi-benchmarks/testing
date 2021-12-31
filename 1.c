//this code is from internet 
// objecftive : discover mpi

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define BASE 10 

int main(int argc, char** argv)
{



	MPI_Init(&argc,&argv);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	
	unsigned long long size_array , i_max ;

	if (argc != 3){
		size_array = 100000;
		i_max = 200000 ;	
	}	
	else{
		char *tmpstring ; 	
		size_array = strtoul(argv[1], &tmpstring, BASE) ;//100000
		i_max = strtoul(argv[2], &tmpstring, BASE) ;	 //200000	
	}
	
	if (world_rank == 0){
		if (argc != 3){

			printf("size of array : %llu \n", size_array) ;
			printf("number iterations of sum : %llu \n", i_max) ;
			printf("Please use arguments : [size of array] [number of sums]\n");		
		}		
	}	

	double * array = malloc( size_array * sizeof(double)) ; 
	double a = 0.0 ; 

	if (world_rank == 0) {
		printf("The number of processes is %d\n" , world_size) ;
		
		// init array
		for (unsigned long long i = 0 ; i < size_array ; i++){
			array[i] = 2.0 ; 
		}
		
		// timer
		struct timespec start , end ; 
		clock_gettime( CLOCK_MONOTONIC, &start ) ; 
			
		for (int i = 1 ; i < world_size ; i++){
			MPI_Send(array, size_array, MPI_DOUBLE, i, i, MPI_COMM_WORLD);
		}

		// time spend in Send execution    	
		clock_gettime( CLOCK_MONOTONIC, &end ) ; 
		printf("%ld spend sharing array\n" , (end.tv_nsec - start.tv_nsec)) ;
		
		for (int i = 1 ; i < world_size ; i++){
			MPI_Recv(array+i, 1, MPI_DOUBLE, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	

		//printf("value computed : %f \n" , array[1]) ;	
	
		// time spend in Send execution    	
		clock_gettime( CLOCK_MONOTONIC, &end ) ; 
		printf("%ld spend sharing array & compute\n" , (end.tv_nsec - start.tv_nsec)) ;
	    	
	} else if (world_rank < world_size ) {
		MPI_Recv(array, size_array, MPI_DOUBLE, 0, world_rank, MPI_COMM_WORLD,
			MPI_STATUS_IGNORE);
	
		// computation \approx same time than recv
		for (unsigned long long i = 0 ; i < i_max ; i++){
			a = a + 1.0 ; 
		}				
		//printf("Process %d received value %d from process 0\n", world_rank , number);
	
		for (int i = 1 ; i < world_size ; i++){
			MPI_Send(&a, 1, MPI_DOUBLE, 0, i, MPI_COMM_WORLD);
		}	
	

	}	
	MPI_Finalize();
}



