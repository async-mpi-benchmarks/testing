#include "covering.h"

MEASUREMENT coverage(int rank, int size, uint64_t NB_BYTES, uint64_t NB_OPERATION)
{
	uint8_t *message = malloc(NB_BYTES*sizeof(uint8_t));
	MEASUREMENT cost;
	cost.rank = 0;
	MPI_Request req = MPI_REQUEST_NULL;
    if(!message)
    {
        cost.mpi_send_cost = 0;
        cost.operation_cost = 0;
        printf("ERROR: could not alocate %lu bytes\n",NB_BYTES);
        return cost;
    }

    if(!rank) //Process 0
    {
 		ticks before,after;
 		
 		//Measurment of Isend cost
 		before = getticks();
        MPI_Isend(message, NB_BYTES, MPI_BYTE, 1, 123, MPI_COMM_WORLD, &req);
        after = getticks();
        cost.mpi_send_cost = after-before;
        
        //Measurment of operation cost
        before = getticks();
        __asm__ volatile(
        		"xor %%rax, %%rax;\n"
        		
        		"loop:;\n"
        			"add $1,%%rax;\n"
        			"cmp %[_b], %%rax;\n"
        			"jne loop;\n"
        		:
        		: //inputs
        			[_b] "r" (NB_OPERATION)
        		:
        			"cc", "memory", "rax"
        	);
        after = getticks();

        cost.operation_cost = after - before;

        //Measurment of Wait cost
        before = getticks();
        MPI_Wait(&req, MPI_STATUS_IGNORE);
        after = getticks();
       	
       	cost.mpi_send_cost += after-before;
    }
    else if(rank)
    {
    	MPI_Recv(message, NB_BYTES, MPI_INT, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    free(message);
    return cost;
}