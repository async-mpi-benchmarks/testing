#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
     int nbl=4 ;
     int nbc =4 ;

    if (rank == 0)
    {  
           srand(time(NULL));

            int **tab1= malloc(nbl*sizeof(int*));

            for( i = 0 ; i < nbl; i++)

            {

                tab1[i] = malloc(nbc*sizeof(int));
                for (int j=0 ; j<nbc ; j++)
                {
                    tab1[i][j] = rand() %100 ;
                }

            }
            
          int **tab2= malloc(nbl*sizeof(int*));

            for(i = 0; i < nbl; i++)

            {

                tab2[i] = malloc(nbc*sizeof(int));
                 for (int j=0 ; j<nbc ; j++)
                {
                    tab2[i][j] = rand() %100;
                }


            }

            // multiplication de deux matrice carré n = 4 
            int **mul= malloc(nbl*sizeof(int*));
             for( i = 0 ; i < nbl; i++)

            {

                mul[i] = malloc(nbc*sizeof(int));
                memset(mul[i],0 ,nbc*sizeof(int)); // Initialiser la matrice mul à 0 
                
            }
            
           
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
        
         for(i = 0; i < nbl; i++)
         {
             MPI_Isend (mul[i] , nbl , MPI_INT , 1 , 000 ,MPI_COMM_WORLD , &req);
         }
         

        rdtsc_fin= rdtsc();
        rdtsc_debut = rdtsc_fin - rdtsc_debut ;
        
         printf("temp du Isend =\n  %llu ,  la fin =  %llu  \n" , rdtsc_debut ,  rdtsc_fin ) ;
          
             for (int i= 0 ; i<nbl ; i++)
             {
                  
                 for (int j=0 ; j<nbc ; j++) 
                 {
                     printf("rank %d, i =  %d \n " , rank ,mul[i][j]);
                 }
                 
               
             }
        MPI_Wait(&req , MPI_STATUS_IGNORE);
        req =MPI_REQUEST_NULL ;
    free(tab1);
    free(tab2);
    }

    if (rank == 1 )
    {
        
        int **mul= malloc(nbl*sizeof(int*));
         for( i = 0 ; i < nbl; i++)

            {

                mul[i] = malloc(nbc*sizeof(int));

            }
        for(i = 0; i < nbl; i++){

                MPI_Irecv(mul[i], nbl, MPI_INT , 0 , 000 ,MPI_COMM_WORLD , &req);


        }
        MPI_Wait (&req , MPI_STATUS_IGNORE);
        
         for (int i = 0 ; i<nbl ; i++)
             {
                 for ( int j=0 ; j<nbc ; j++)
                 {
                    printf("rank %d, mul =  %d \n " , rank ,mul[i][j]);
                 }
                
             }
            
            free(mul);
            

    }
    MPI_Finalize();
}