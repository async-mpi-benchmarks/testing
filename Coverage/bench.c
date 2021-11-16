#include "covering.h"

#define TT_BYTES 1048576
#define TT_OP 100000
#define NB_REP 100

int main(int argc, char **argv)
{
	int rank,size;

	//MPI initilalisation
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MEASUREMENT cost;
    
	ticks sum_op, sum_by;
	sum_op = sum_by = 0;
	FILE *F=NULL;
	if(!rank)
	{
		F=fopen("bench.dat","w");
	}
	for(unsigned long NB_BYTES = 1; NB_BYTES<=TT_BYTES; NB_BYTES = NB_BYTES * 2)
	{
		for(unsigned long NB_OP = 1; NB_OP<TT_OP; NB_OP+=5)
		{
			sum_op = sum_by = 0;
		    for(unsigned long i = 0; i<NB_REP; i++)
		    {
		    	cost = coverage(rank,size,NB_BYTES,NB_OP);
		    	if(!rank)
		    	{
		    		sum_op+=cost.operation_cost;
		    		sum_by+=cost.mpi_send_cost;
		    	}
		    }
		    if(!rank)
		    {
		   		sum_op = sum_op/NB_REP;
		    	sum_by = sum_by/NB_REP;
		    	fprintf(F,"rdtsc: %llu NB_BYTES: %lu rdtsc_OP: %llu covering: %lf%%\n",sum_by, NB_BYTES, sum_op,((double)sum_op/(double)sum_by)*100);
		    	printf("%lu bytes on %d; %lu op on %d\n",NB_BYTES,TT_BYTES,NB_OP,TT_OP);
		    }
		}
	}
	if(!rank)
		fclose(F);
	
	MPI_Finalize();

	return 0;
}