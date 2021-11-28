// this is an example of calculation of mathematical factorial
// n ! = (n-1)! x n
// idea : want to compute large factorial 
// 10! = ( 1 x 2 ) x ( 3 x 4 ) x ( 5 x 6 ) .... in each cores 

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc , char** argv){

	int rank , size ; 
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int width = 2 ; 
	unsigned long long result, tmp, sresult, debut, sdebut, fin , sfin; 
	// main mpi process ;
	if (rank == 0){
		result = 1 ; 
		debut = 0 ; 
		fin = 0 ; 
		tmp = 0 ; 
		
		for (int i = 0 ; i < 10 ; ++i)
		{
			printf("i : %i\n", i);
			debut = i * width +1; 
			fin = (i+1) * width  ;
			MPI_Send(&debut, 1, MPI_UNSIGNED_LONG_LONG, i+1, 0, MPI_COMM_WORLD);
			MPI_Send(&fin, 1, MPI_UNSIGNED_LONG_LONG, i+1, 1, MPI_COMM_WORLD);
		}
		
		// recieve the result 
		for (int i = 0 ; i < 10 ; ++i)
		{

			MPI_Recv(&tmp, 1, MPI_UNSIGNED_LONG_LONG, i+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("recieve from %i : %lli\n", i,  tmp);			
			result= result * tmp ; 
			printf(" result : %lli\n", result ) ;			
		}
		printf(" result : %lli\n", result ) ;		
		
		// verfication
		tmp = 1 ; 
		for (int i = 1 ; i <= fin; ++i){
			tmp *= i ; 
		}
		printf(" verification result : %lli\n", tmp ) ;				
	}
	
	else if (rank < 11)
	{
		printf("init rank : %i\n", rank);
		sresult = 1; 
		MPI_Recv(&sdebut, 1, MPI_UNSIGNED_LONG_LONG, 0,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
		MPI_Recv(&sfin, 1, MPI_UNSIGNED_LONG_LONG, 0,1, MPI_COMM_WORLD, MPI_STATUS_IGNORE );	
		printf("rank : %i, debut : %lli, fin : %lli\n", rank, sdebut, sfin);
		// calculation of the sub factoriel
		
		for (int i = sdebut; i <= sfin ; i++){
			sresult *=i ; 
		}
		MPI_Send(&sresult, 1, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD);		
		
		
				
	}
	MPI_Finalize();
	

}
