#include "covering.h"

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("ERROR: need arguments\nUSAGE: %s [number of bytes] [number of simple operation]\n",argv[0]);
        return 1;
	}

	int rank,size;

	//MPI initilalisation
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    uint64_t NB_BYTES = strtoull(argv[1],NULL,10);
    
    MEASUREMENT cost = coverage(rank,size,NB_BYTES,strtoull(argv[2],NULL,10));

	if(!cost.mpi_send_cost && !cost.operation_cost && !rank)
	{
		MPI_Finalize();
		return 1;
	}
	if(!rank)
	{
		printf("rdtsc: %llu NB_BYTES: %lu rdtsc_OP: %llu covering: %lf%%\n",cost.mpi_send_cost, NB_BYTES, cost.operation_cost,((double)cost.operation_cost/(double)cost.mpi_send_cost)*100);
	}

	MPI_Finalize();
	return 0;
}