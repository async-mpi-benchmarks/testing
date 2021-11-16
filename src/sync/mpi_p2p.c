#include <mpi.h>
#include <stdio.h>

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
        printf("Process %d: sending tokenber `%d` to process 1\n", world_rank, token);
        MPI_Send(&token, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else if (world_rank == 1) {
        MPI_Recv(&token, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d: received tokenber `%d` from process 0\n", world_rank, token);
    }

    MPI_Finalize();
}
