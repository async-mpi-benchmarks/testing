#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc,&argv);

    int rank , P;
    MPI_Comm_size(MPI_COMM_WORLD,&P);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int buff=3;
    if(rank==0)
    {
        MPI_Send(&buff,1,MPI_INT,1,123,MPI_COMM_WORLD);
        printf("processor %d  sent %d\n",rank,buff);
        //simulating computation 
        nanosleep()
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Recv(&buff,1,MPI_INT,1,123,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    else
    {
        MPI_Recv(&buff,1,MPI_INT,0,123,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        printf("processor %d  sent %d\n",rank,buff);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Send(&buff,1,MPI_INT,0,123,MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
