#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include "cycle.h"

int main(int argc, char** argv)
{
    if(argc!=2)
    {
        printf("ERROR: need arguments\nUSAGE: %s [number of bytes]\n",argv[0]);
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
    
    //MPI initilalisation
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Request req = MPI_REQUEST_NULL;

    if(!rank)   //Process 0
    {
        for(int i = 0; i<NB_BYTES;i++)
        {
            message[i] = rank;  //Message initialisation
        }

        printf("%d Envoie %llu bytes\n", rank,NB_BYTES);
        ticks total,tmp;
        
        //Time cost mesurement of a non-blocking send for process 0
        ticks before = getticks();
        MPI_Isend(message, NB_BYTES, MPI_BYTE, 1, 123, MPI_COMM_WORLD, &req);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
        ticks after = getticks();
        total = after - before;
        req = MPI_REQUEST_NULL;

        //Time cost mesurement of a receive for process 0
        before = getticks();
        MPI_Recv(message, NB_BYTES, MPI_INT, 1, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
        after = getticks();
        
        printf("%d Reception %llu bytes\n",rank, NB_BYTES);

        total += after - before;    //Total ticks cost
        
        //Receiving of the time cost from the process 1 
        MPI_Recv(&tmp, 1, MPI_LONG_LONG_INT, 1, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        total += tmp; 

        printf("rdtsc: %llu\n",after - before);
    }
    else if(rank == 1)  //process 1
    {
        ticks before,after,total;

       //Time cost mesurement of a receive for process 1
        before = getticks();
        MPI_Recv(message, NB_BYTES, MPI_INT, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
        after = getticks();
        total = after - before;
        
        printf("%d Reception %llu bytes\n",rank, NB_BYTES);
        req = MPI_REQUEST_NULL;

        for(int i = 0; i<NB_BYTES;i++)
        {
            message[i] += rank;     //Message modification
        }

        printf("%d Envoie %llu bytes\n", rank,NB_BYTES);

        //Time cost mesurement of a non-blocking send for process 1
        before = getticks();
        MPI_Isend(message, NB_BYTES, MPI_BYTE, 0, 123, MPI_COMM_WORLD, &req);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
        after = getticks();
        total += after - before;
        
        //Sending total cost mesurement of MPI operation of process 1 to process 0
        MPI_Send(&total,1,MPI_LONG_LONG_INT,0,123,MPI_COMM_WORLD);
    }

    free(message);
    MPI_Finalize();
    
    return 0;
}
