#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

const int LOOPING_LIMIT = 16;

int main()
{
    MPI_Init(NULL, NULL);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Assert that there are at least two processes running
    if (world_size < 2) {
        fprintf(stderr, "Error: world size must be at least 2\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int looping_count = 0;

    // Continue looping while we have not reached the limit
    while (looping_count < LOOPING_LIMIT) {
        MPI_Request req;

        // If process is NOT 0, receive the loop counter from your predecessor
        if (world_rank != 0) {
            MPI_Irecv(&looping_count, 1, MPI_INT, world_rank - 1, 0,
                      MPI_COMM_WORLD, &req);

            // Do some heavy computation here
            sleep(1);

            MPI_Wait(&req, MPI_STATUS_IGNORE);
            printf("Process %d: received token from process %d\n", 
                   world_rank, world_rank - 1);
        } else { // Process 0 increments the loop counter
            looping_count += 1;
        }

        // Whichever process you are, send the loop counter
        MPI_Isend(&looping_count, 1, MPI_INT, (world_rank + 1) % world_size, 0,
                 MPI_COMM_WORLD, &req);

        // Do some heavy computation here
        sleep(2);

        MPI_Wait(&req, MPI_STATUS_IGNORE);

        // If you are process 0, receive the loop counter from the last
        // process in the world
        if (world_rank == 0) {
            MPI_Irecv(&looping_count, 1, MPI_INT, world_size - 1, 0,
                     MPI_COMM_WORLD, &req);

            // Do some heavy computation here
            sleep(1);

            MPI_Wait(&req, MPI_STATUS_IGNORE);
            printf("Process %d: received token from process %d, loop #%d\n", 
                   world_rank, world_size - 1, looping_count);
        }
    }
        
    MPI_Finalize();
}
