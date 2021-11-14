#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "cycle.h"

typedef struct measurement
{
	ticks mpi_send_cost,operation_cost;
	int rank;
}  MEASUREMENT;

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
    	for(int i = 0; i<NB_BYTES;i++)
 		{
 			message[i] = rank;      //Message initialisation
 		}

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
        			"inc %%rax;\n"
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
    return cost;
}



#ifndef MAIN 
#define MAIN

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

#endif