#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include "cycle.h"

//argv[0] nom du programme
//argv[1] 10

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

    MPI_Request req = MPI_REQUEST_NULL;

    if(!rank)
    {
        for(int i = 0; i<NB_BYTES;i++)
        {
            message[i] = rank;
        }

        

        printf("%d Envoie %llu bytes\n", rank,NB_BYTES);
        ticks total,tmp;
        ticks before = getticks();
        MPI_Isend(message, NB_BYTES, MPI_BYTE, 1, 123, MPI_COMM_WORLD, &req);
        
        MPI_Wait(&req, MPI_STATUS_IGNORE);
        ticks after = getticks();
        total = after - before;
        req = MPI_REQUEST_NULL;

        before = getticks();
        MPI_Recv(message, NB_BYTES, MPI_INT, 1, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        

        MPI_Wait(&req, MPI_STATUS_IGNORE);
        after = getticks();
        printf("%d Reception %llu bytes\n",rank, NB_BYTES);

        total += after - before;
        MPI_Recv(&tmp, 1, MPI_LONG_LONG_INT, 1, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        total += tmp; 

        printf("rdtsc: %llu\n",after - before);
    }
    else if(rank == 1)
    {
        ticks before,after,total;

        before = getticks();
        MPI_Recv(message, NB_BYTES, MPI_INT, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
        after = getticks();
        total = after - before;
        printf("%d Reception %llu bytes\n",rank, NB_BYTES);
        req = MPI_REQUEST_NULL;

        for(int i = 0; i<NB_BYTES;i++)
        {
            message[i] += rank;
        }

        printf("%d Envoie %llu bytes\n", rank,NB_BYTES);

        before = getticks();
        MPI_Isend(message, NB_BYTES, MPI_BYTE, 0, 123, MPI_COMM_WORLD, &req);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
        after = getticks();
        total += after - before;
        MPI_Send(&total,1,MPI_LONG_LONG_INT,0,123,MPI_COMM_WORLD);
    }

    free(message);
    MPI_Finalize();
    return 0;
}
