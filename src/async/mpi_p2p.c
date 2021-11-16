#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    MPI_Init(NULL, NULL);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Assert that there are only two processes running
    if (world_size != 2) {
        fprintf(stderr, "Error: world size is %d but must be 2\n", world_size);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int token = 0;

    // Send if you are process 0, receive if you are process 1
    if (world_rank == 0) {
        token = 42;
        printf("Process %d: sending token `%d` to process 1\n", world_rank, token);

        MPI_Request req;
        MPI_Isend(&token, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &req);

        // Simulate doing an expensive computation
        printf("Process %d: performing expensive computation...\n", world_rank);
        sleep(3);
        printf("Process %d: finished expensive computation\n", world_rank);

        MPI_Wait(&req, MPI_STATUS_IGNORE);
        printf("Process %d: sent token\n", world_rank);
    } else if (world_rank == 1) {
        MPI_Request req;
        MPI_Irecv(&token, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);

        // Simulate doing an expensive computation
        printf("Process %d: performing expensive computation...\n", world_rank);
        sleep(1);
        printf("Process %d: finished expensive computation\n", world_rank);

        MPI_Wait(&req, MPI_STATUS_IGNORE);
        printf("Process %d: received token `%d` from process 0\n", world_rank, token);
    }

    MPI_Finalize();
}
