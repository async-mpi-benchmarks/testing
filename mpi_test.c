#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main (int argc , char** argv)
{
    int rank , size ;
    MPI_Init(&argc , &argv);
    MPI_Comm_rank(MPI_COMM_WORLD ,&rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
     // Opérations bloquantes
    if (rank == 0)
    {  
       
        int nbr;
 
         printf("Entrez un nombre pour calculer sa factorielle\n");
         scanf("%d", &nbr);

         MPI_Send (&nbr , 1 , MPI_INT , 1 , 000 ,MPI_COMM_WORLD );
         printf("rank %d ,  j'envoie le nombre  %d \n" , rank ,  nbr ) ;
      

        

    }
    if (rank == 1 )
    {
        // redéclarer les variables qu'on reçois 
        int  i,nbr , f = 1;
       
        MPI_Recv(&nbr , 1 , MPI_INT , 0 , 000 ,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (i = 1; i <= nbr; i++)
          {
               f = f * i;
          }
          printf("rank %d ,  j'ai reçu le  nombre  %d \n" , rank ,  nbr ) ;
          printf("Calcul de factorielle  : \n " );
          printf("Factorielle de %d = %d\n", nbr, f);


    }
    MPI_Finalize();
}