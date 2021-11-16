#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

const int PING_PONG_LIMIT = 16;

int main() {
    MPI_Init(NULL, NULL);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Assert that there are only two processes running
    if (world_size != 2) {
        fprintf(stderr, "Error: world size must be 2\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int ping_pong_count = 0;
    int partner_rank = (world_rank + 1) % 2;

    // Continue sending and receiving while we have not reached the limit
    while (ping_pong_count < PING_PONG_LIMIT) {
        MPI_Request req;
        if (world_rank == ping_pong_count % 2) {
            ping_pong_count += 1;

            MPI_Isend(&ping_pong_count, 1, MPI_INT, partner_rank, 0,
                      MPI_COMM_WORLD, &req);

            // Simulate doing an expensive computation
            sleep(1);

            MPI_Wait(&req, MPI_STATUS_IGNORE);
            printf("Process %d: ping `%d` process %d\n",
                   world_rank, ping_pong_count, partner_rank);
        } else {
            MPI_Irecv(&ping_pong_count, 1, MPI_INT, partner_rank, 0,
                      MPI_COMM_WORLD, &req);

            // Simulate doing an expensive computation
            sleep(1);

            MPI_Wait(&req, MPI_STATUS_IGNORE);
            printf("Process %d: pong `%d` process %d\n",
                   world_rank, ping_pong_count, partner_rank);
        }
    }
    
    MPI_Finalize();
}
