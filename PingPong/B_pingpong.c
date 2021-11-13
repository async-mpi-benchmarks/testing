#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "cycle.h"


int main(int argc, char** argv)
{
    if(argc!=2)
    {
        printf("ERROR: need arguments\n");
        return 1;
    }

    unsigned long long NB_BYTES = strtoull(argv[1],NULL,10);

    uint8_t *message = malloc(NB_BYTES*sizeof(uint8_t));

    if(!message)
    {
        printf("ERROR: could not alocate %llu bytes\n",NB_BYTES);
        return 2;
    }

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(!rank)
    {
    	for(int i = 0; i<NB_BYTES;i++)
 		{
 			message[i] = rank;
 		}
        ticks total, tmp;
    	printf("%d Envoie %llu bytes\n", rank,NB_BYTES);
    	ticks before = getticks();
        MPI_Send(message, NB_BYTES, MPI_BYTE, 1, 123, MPI_COMM_WORLD);
        ticks after = getticks();
    	total = after - before;

        before = getticks();
        MPI_Recv(message, NB_BYTES, MPI_INT, 1, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    	after = getticks();
        printf("%d Reception %llu bytes\n",rank, NB_BYTES);
        
        total += after - before;
        MPI_Recv(&tmp, 1, MPI_LONG_LONG_INT, 1, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        total += tmp; 
        printf("rdtsc: %llu\n",tmp);

    }
    else if(rank == 1)
    {
    	ticks total, before, after;
        before = getticks();
        MPI_Recv(message, NB_BYTES, MPI_INT, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    	after = getticks();
        total = after - before;
        printf("%d Reception %llu bytes\n",rank, NB_BYTES);

        for(int i = 0; i<NB_BYTES;i++)
        {
            message[i] += rank;
        }

    	printf("%d Envoie %llu bytes\n", rank,NB_BYTES);
        before = getticks();
    	MPI_Send(message, NB_BYTES, MPI_BYTE, 0, 123, MPI_COMM_WORLD);
        after = getticks();
        total += after-before;

        MPI_Send(&total,1,MPI_LONG_LONG_INT,0,123,MPI_COMM_WORLD);
    }

    free(message);
    MPI_Finalize();
	return 0;
}
