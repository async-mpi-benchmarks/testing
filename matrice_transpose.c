/* 
	test la collective MPI_Ialltoall
Transposition de la matrice processus × données
Chaque processus du communicateur envoie des informations
différentes à l’ensemble des processus du communicateur, et
reçoit des informations différentes de tous les processus du
communicateur
exemple : 
	
	p1 : 1	2   3 						p1 : 1	6   0 						
	p2 : 6	9	7 ==> matrice trasposé  p2 : 2	9	5 
	p3 : 0	5	4						p3 : 3	7	4
	 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>


int main(int argc, char **argv)
{
    
    int rank, P;

    MPI_Init(&argc, &argv);
    
    MPI_Request req ; 

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    srand(getpid());

    double  *data=(double*)malloc(P*sizeof(double));
    double *proc=(double*)malloc(P*sizeof(double));

    /* Remplir et afficher tableau "data" */
    printf("data[%d] = [ ",rank);
	 for (int i=0 ; i<P ; i++)
	 {
	 	data[i]=rand()%100;
	 	printf("%lf, " , data[i]);
	 }
	 printf(") ];\n\n");

	 

	MPI_Ialltoall(data,1, MPI_DOUBLE,
       			 proc, 1, MPI_DOUBLE,
       			 MPI_COMM_WORLD, 
       			 &req);
	MPI_Wait(&req,MPI_STATUS_IGNORE);

	double *matrice_trans=NULL;
	req= MPI_REQUEST_NULL;

	if (rank==0)
	{
		matrice_trans=(double*)malloc(P*P*sizeof(double));
	}

	MPI_Igather(proc, P, MPI_DOUBLE,
       			matrice_trans,P, MPI_DOUBLE,
       			0, MPI_COMM_WORLD,&req);
	MPI_Wait(&req,MPI_STATUS_IGNORE);

	/* l'affichage de la matrice transposé */

	if (rank==0)
	{
		printf("matrice_trans[%d] = [ ",rank);
	 for (int i=0 ; i<P; i++)
	 {
	 	for(int j = 0; j<P; j++)
	 		printf("%lf " ,matrice_trans[j+i*P]);
	 	printf("\n");
	 }
	 printf("];\n");

	}

    free(data);
    free(proc);
    free(matrice_trans);
   
    MPI_Finalize();

    return 0;
}