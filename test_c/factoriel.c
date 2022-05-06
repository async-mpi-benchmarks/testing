#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc, char *argv[]){
    int rank;
    int size;
    int fact,n,borne_inf,borne_sup;
    double res_loc = 1.0;
    double res;


    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Request req1,req2; 

    if(rank==0){
        srand(getpid());
        n = rand()%100;
    }

    MPI_Ibcast(&n,1,MPI_INT,0,MPI_COMM_WORLD,&req1);
    MPI_Ibarrier(MPI_COMM_WORLD,&req1);
    MPI_Wait(&req1,MPI_STATUS_IGNORE);

    // Calculate the bounds for each process to define their local areas.
    borne_inf = rank * (n / size) + 1;
    if(rank==(size-1))
        borne_sup= n;
    else
        borne_sup = (rank + 1) * (n / size);

    // Multiplication in local area
     
    for(int i=borne_inf;i<=borne_sup;i++){
        res_loc = res_loc * (double)i;
    }
    //Multiplication of all local results 
    MPI_Ireduce(&res_loc,&res,1,MPI_DOUBLE,MPI_PROD,0,MPI_COMM_WORLD,&req2);
    MPI_Ibarrier(MPI_COMM_WORLD,&req1);
    MPI_Wait(&req2,MPI_STATUS_IGNORE);
    
    // Process with rank 0 prints the result 
    if(rank==0){
        printf("The factorial of %d is %lf \n",n,res );
    }
    MPI_Finalize();
    return 0;
    }