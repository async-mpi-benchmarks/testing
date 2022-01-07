#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

unsigned long long rdtsc()
{
	unsigned long long rdtsc;

	__asm__ volatile( "rdtsc;\n" : "=A" (rdtsc));

	return rdtsc;
}

void print_mat(double*mat,int TAILLE_MAT)
{
	for (int i = 0; i < TAILLE_MAT; ++i)
	{
		for(int j = 0; j<TAILLE_MAT;j++)
			printf(" %lf",mat[j+i*TAILLE_MAT]);
		printf("\n");
	}
	printf("\n");
}

void *mat_trans(double*mat,int TAILLE_MAT)
{
	double * tmp = malloc(TAILLE_MAT*TAILLE_MAT*sizeof(double));
	for(int i = 0; i<TAILLE_MAT;i++)
	{
		for(int j = 0; j<TAILLE_MAT;j++)
		{
			tmp[j+i*TAILLE_MAT] = mat[i+j*TAILLE_MAT];
		}
	}
	printf("Transpose: \n");
	print_mat(tmp,TAILLE_MAT);
	print_mat(mat,TAILLE_MAT);
	return tmp;
}

void *mat_mult(double *mat_A,double *mat_B,int TAILLE_MAT)
{
	double *result = malloc(TAILLE_MAT*TAILLE_MAT*sizeof(double));
	for (int i = 0; i < TAILLE_MAT; ++i)
	{
		for(int j = 0; j<TAILLE_MAT; j++)
		{
			result[j+i*TAILLE_MAT] = 0;
			for(int k = 0; k<TAILLE_MAT;k++)
			{
				result[j+i*TAILLE_MAT] += mat_A[k+i*TAILLE_MAT] * mat_B[j+k*TAILLE_MAT];
			}
		}
	}
	return result;
}

int main(int argc, char** argv)
{
	int rank, size;
  	MPI_Init(&argc, &argv);	//initialisation de l'utilisation des appel MPI
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //création du rank (chaque processus pocède son propre rank)
  	MPI_Comm_size(MPI_COMM_WORLD, &size);	//permet de réuperer le nombre de processus sur le meme comm

  	int TAILLE_MAT = 10;
  	double * mat_A = malloc(TAILLE_MAT*TAILLE_MAT*sizeof(double));
  	double * mat_B = malloc(TAILLE_MAT*TAILLE_MAT*sizeof(double));
  	double * mat_C = malloc(TAILLE_MAT*TAILLE_MAT*sizeof(double));
  	double * mat_D;
  	if(!rank)
  	{
  		MPI_Request req = MPI_REQUEST_NULL;
  		srand(getpid());
  		for(int i = 0; i < TAILLE_MAT*TAILLE_MAT; i++)
  		{
  			mat_A[i] = rand()%100;
  		}

  		MPI_Send(mat_A,TAILLE_MAT*TAILLE_MAT, MPI_DOUBLE, 1,123, MPI_COMM_WORLD);
  		
  		MPI_Recv(mat_B,TAILLE_MAT*TAILLE_MAT,MPI_DOUBLE,1,456,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  		MPI_Irecv(mat_C,TAILLE_MAT*TAILLE_MAT,MPI_DOUBLE,1,789,MPI_COMM_WORLD,&req);

  		mat_D = mat_mult(mat_A,mat_B,TAILLE_MAT);

  		MPI_Wait(&req,MPI_STATUS_IGNORE);

  		print_mat(mat_D,TAILLE_MAT);
  		print_mat(mat_C,TAILLE_MAT);
  	}
  	else
  	{
  		srand(getpid());
  		MPI_Request req = MPI_REQUEST_NULL;
  		MPI_Irecv(mat_A,TAILLE_MAT*TAILLE_MAT,MPI_DOUBLE,0,123,MPI_COMM_WORLD,&req);
  		for(int i = 0; i<TAILLE_MAT*TAILLE_MAT;i++)
  		{
  			mat_B[i] = rand()%100;
  		}
  		MPI_Wait(&req,MPI_STATUS_IGNORE);
  		req = MPI_REQUEST_NULL;
  		MPI_Isend(mat_B,TAILLE_MAT*TAILLE_MAT,MPI_DOUBLE,0,456,MPI_COMM_WORLD,&req);
  		mat_A = mat_trans(mat_A,TAILLE_MAT);
  		MPI_Wait(&req,MPI_STATUS_IGNORE);
  		mat_A = mat_mult(mat_A,mat_B,TAILLE_MAT);
  		MPI_Send(mat_A,TAILLE_MAT*TAILLE_MAT,MPI_DOUBLE,0,789,MPI_COMM_WORLD);
  	}
  	free(mat_A);
  	free(mat_B);
  	MPI_Finalize();
}