#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

unsigned long long  rdtsc()
{ 
    unsigned long long rdtsc ;
     __asm__ volatile ( "rdtsc; \n" :"=A" (rdtsc));
     return rdtsc ;
}

int main (int argc , char** argv)
{
     unsigned long long rdtsc_debut ; 
     unsigned long long rdtsc_fin ; 

    int rank , size ;
    int i , j ,k;
    MPI_Init(&argc , &argv);
    MPI_Comm_rank(MPI_COMM_WORLD ,&rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Request req = MPI_REQUEST_NULL ;

     // Opérations non bloquantes
    if (rank == 0)
    {  
            int nbl ,nbc =4 ;

            int **tab1= malloc(nbl*sizeof(int));

            for( i = 0 ; i < nbl; i++)

            {

                tab1[i] = malloc(nbc*sizeof(int));

            }
          int **tab2= malloc(nbl*sizeof(int));

            for(i = 0; i < nbl; i++)

            {

                tab2[i] = malloc(nbc*sizeof(int));

            }

            // multiplication de deux matrice carré n = 4 
            int **mul= malloc(nbl*sizeof(int));
            mul[i][j]=0;
             for( i = 0; i < nbl; i++)
            {
                 for( j = 0; j < nbc; j++)
                 {
                       
                     for( k = 0; k < nbc; k++)
                    {
                      mul[i][j] += tab1[i][k] * tab2[k][j];
                    }
                 }
            }
       
        rdtsc_debut = rdtsc();
        int **tab1;
        int **tab2;
        int **mul;
        
         MPI_Isend (tab1[i] , 4 , MPI_INT , 1 , 000 ,MPI_COMM_WORLD , &req);

        rdtsc_fin= rdtsc();
        rdtsc_debut = rdtsc_fin - rdtsc_debut ;
        
         printf("temp du Isend =  %llu ,  la fin =   %llu " , rdtsc_debut ,  rdtsc_fin ) ;
      
             for (int j = 0 ; j<4 ; j++)
             {
                 printf("rank %d, i =  %d \n " , rank ,mul[i][j]);
               
             }
        MPI_Wait(&req , MPI_STATUS_IGNORE);
        req =MPI_REQUEST_NULL ;

    }

    if (rank == 1 )
    {
        int **mul;
        int **mul= malloc(nbl*sizeof(int));
        

        MPI_Irecv(mul[i][j] , 4, MPI_INT , 0 , 000 ,MPI_COMM_WORLD , &req);

         for (int i = 0 ; j<nbl ; i++)
             {
                 
                 printf("rank %d, mul =  %d \n " , rank ,mul[i][j]);
             }
            free(tab1);
            free(tab2);
            free(mul);
            

    }
    MPI_Finalize();
}