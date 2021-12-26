// this is an example of calculation of mathematical factorial
// n ! = (n-1)! x n
// idea : want to compute large factorial 
// 10! = ( 1 x 2 ) x ( 3 x 4 ) x ( 5 x 6 ) .... in each cores 

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <gmp.h>

int main(int argc , char** argv){

	int rank , size ; 
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int width = 3 ; 
	unsigned long long tmp, debut, sdebut, fin , sfin; 
	unsigned long sword[20];
	unsigned long word[20];
	
	for (int i = 0 ; i < 20 ; ++i){
		word[i] = 0 ;
		sword[i] = 0 ; 
	}
		
	mpz_t result ; 
	mpz_init (result) ; 
	mpz_set_ui(result,1);

	mpz_t mpz_tmp ; 
	mpz_init (mpz_tmp) ; 
	mpz_set_ui(mpz_tmp,1);


	mpz_t sresult ; 
	mpz_init (sresult) ; 
	mpz_set_ui(sresult,1);
	
 
	
	// main mpi process ;
	if (rank == 0){
		//result = 1 ; 
		debut = 0 ; 
		fin = 0 ; 
		tmp = 0 ; 
		
		for (int i = 0 ; i < size -1 ; ++i)
		{
			printf("i : %i\n", i);
			debut = i * width +1; 
			fin = (i+1) * width  ;
			MPI_Send(&debut, 1, MPI_UNSIGNED_LONG_LONG, i+1, 0, MPI_COMM_WORLD);
			MPI_Send(&fin, 1, MPI_UNSIGNED_LONG_LONG, i+1, 1, MPI_COMM_WORLD);
		}
		
		// recieve the result 
		for (int i = 0 ; i < size -1 ; ++i)
		{
			for (int i = 0 ; i < 20 ; ++i){
				word[i] = 0 ; 
			}
			MPI_Recv(&word, 20, MPI_UNSIGNED_LONG, i+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("recieved word : %li %li\n", word[0], word[1] );			
			mpz_set_ui(mpz_tmp,0);	
			mpz_import(mpz_tmp, 20, -1, sizeof(sword[0]), 0, 0, word);		
		
			mpz_mul(result, result, mpz_tmp);
			gmp_printf(" recieve  : %Zd\n", mpz_tmp )	;				
			//printf(" result : %lli\n", result ) ;			
		}
		//printf(" result : %lli\n", result ) ;	
		gmp_printf(" result : %Zd\n", result )	;
		
		// verfication
		mpz_set_ui(mpz_tmp,1);
		mpz_set_ui(result,1);
		for (int i = 1 ; i <= fin; ++i){
			mpz_mul_ui(result, result, i);
		}
		gmp_printf(" verification result : %Zd\n", result )	;			
	}
	
	
	// compute p
	else if (rank < size )
	{
		printf("init rank : %i\n", rank);
		mpz_set_ui(sresult,1); 
		MPI_Recv(&sdebut, 1, MPI_UNSIGNED_LONG_LONG, 0,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
		MPI_Recv(&sfin, 1, MPI_UNSIGNED_LONG_LONG, 0,1, MPI_COMM_WORLD, MPI_STATUS_IGNORE );	
		printf("rank : %i, debut : %lli, fin : %lli\n", rank, sdebut, sfin);
		// calculation of the sub factoriel
		
		for (int i = sdebut; i <= sfin ; i++){
			mpz_mul_ui(sresult, sresult, i);
		}
		
		for (int i = 0 ; i < 20 ; ++i){
			sword[i] = 0 ; 
		}
		gmp_printf(" send result : %Zd\n", sresult )	;
		mpz_export(sword, NULL, -1, sizeof(sword[0]), 0, 0, sresult);
		
		printf("sword : %li %li\n", sword[0] , sword[1]);
		
		MPI_Send(&sword, 20, MPI_UNSIGNED_LONG, 0, 0, MPI_COMM_WORLD);		
		
		
				
	}
	MPI_Finalize();
	

}
