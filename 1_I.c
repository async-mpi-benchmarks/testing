// Objective : quantify time and evaluate covering effect on timings
// between sync and async paradigm through MPI

/*
Protocol : 
	- send array from rank 0 to each working process
	- compute \sum_0^N 1 on each process to enforce computation on the cpu core
	- send back the sum result 
	
	- Array sending and sum computation can overlap because they are
		 independants.
	- This is the async example (without overlap between the 2 independant phases)
	- Hypothesis : his sync implementation is more efficient than the async (1_I)
*/


#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define BASE 10 

inline
unsigned long long sync_rdtscp(void)
{
  unsigned long long a, d;
  __asm__ volatile ("cpuid");  
  __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
  
  return (d << 32) | a;
}

inline
unsigned long long rdtsc(void)
{
  unsigned long long a, d;
 
  __asm__ volatile ("rdtsc" : "=a" (a), "=d" (d));
  
  return (d << 32) | a;
}


int main(int argc, char** argv)
{
	MPI_Init(&argc,&argv);
	
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Status special_status ;	
	MPI_Request special_request = MPI_REQUEST_NULL ;
	
	
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

		if (size_array < world_size){
			printf("Please use size of array >= %d\n", world_size);
		}		
				
	}		

	if (size_array < world_size){
		size_array = world_size + 1 ;
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
		unsigned long long start = rdtsc();	
			
		for (int i = 1 ; i < world_size ; i++){
		    	MPI_Isend(array, size_array, MPI_DOUBLE, i, i, MPI_COMM_WORLD, &special_request);
	    	}
	    
		for (int i = 1 ; i < world_size ; i++){
			MPI_Wait(&special_request , &special_status);
		}
	
		unsigned long long end = rdtsc();

	
		for (int i = 1 ; i < world_size ; i++){
		    	MPI_Irecv(array+i, 1, MPI_DOUBLE, i, i, MPI_COMM_WORLD , &special_request);
	    	}
	    
		for (int i = 1 ; i < world_size ; i++){
			MPI_Wait(&special_request , &special_status); 
		}	
		
		//printf("value computed : %f \n" , array[1]) ;
						    
		// time spend in Send execution  	    	
		unsigned long long end2 = rdtsc();
		printf("%llu spend sharing array\n" , (end - start)) ;		
		printf("%llu spend sharing array & compute\n" , (end2 - start)) ;	    	
	    	
	} else if (world_rank < world_size ) {	
		MPI_Irecv(array, size_array, MPI_DOUBLE, 0, world_rank, MPI_COMM_WORLD,
			&special_request);

		// computation \approx same time than recv			
		for (unsigned long long i = 0 ; i < i_max ; i++){
			a = a + 1.0 ; 
		}	
			
		MPI_Wait(&special_request , &special_status ) ; 
		
		for (int i = 1 ; i < world_size ; i++){
			MPI_Isend(&a, 1, MPI_DOUBLE, 0, i, MPI_COMM_WORLD, &special_request) ;
			MPI_Wait(&special_request , &special_status) ; 
		}
		
		//printf("Process %d received value %d from process 0\n", world_rank , number);
 		
	}	
	MPI_Finalize();
}



