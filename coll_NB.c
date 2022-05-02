#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>

int main(int argc,char** argv){
        int rank,size,N;
        int* Send_buf=NULL;
        MPI_Request req1,req2;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        MPI_Comm_size(MPI_COMM_WORLD,&size);

        N=10;
        if(rank==0)
        {
                printf("Master creating data...\n");
                Send_buf=(int*)malloc(sizeof(int)*size*N);
                for(int i=0;i<10*size;i++)
                        Send_buf[i]=i;
        }
        int* Recv_buf=(int*)malloc(sizeof(int)*N);
        MPI_Iscatter(Send_buf,N,MPI_INT,Recv_buf,N,MPI_INT,0,MPI_COMM_WORLD,&req1);
        MPI_Wait(&req1,MPI_STATUSES_IGNORE);
        double avg_loc=0;
        for(int i=0;i<N;i++)
        {
                avg_loc+=Recv_buf[i];
        }
        avg_loc/=N;
        
        printf("%d calculates the local average %f\n",rank,avg_loc);
        double avg_glob;
        MPI_Ireduce(&avg_loc,&avg_glob,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD,&req2);
        MPI_Wait(&req2,MPI_STATUSES_IGNORE);
        if(rank==0){
                printf("The average is : %f\n",avg_glob/size);
        }
        
        MPI_Finalize();
        return EXIT_SUCCESS;
}

