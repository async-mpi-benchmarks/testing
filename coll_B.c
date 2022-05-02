#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>

int main(int argc,char** argv){
        int rank,size,N;
        int* Send_buf=NULL;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        MPI_Comm_size(MPI_COMM_WORLD,&size);

        N=10;
        if(rank==0){
                        printf("Master creating data...\n");
                        Send_buf=(int*)malloc(sizeof(int)*size*N);
                        for(int i=0;i<10*size;i++)
                                Send_buf[i]=i;
        

        }
        int* Recv_buf=(int*)malloc(sizeof(int)*N);
        MPI_Scatter(Send_buf,N,MPI_INT,Recv_buf,N,MPI_INT,0,MPI_COMM_WORLD);

        double avg_loc=0.0;
        for(int i=0;i<N;i++)
                avg_loc+=Recv_buf[i];
        avg_loc/=N;
        printf("%d calculates subavg as %f\n",rank,avg_loc);
        double avg_glob;
        //wait
        MPI_Reduce(&avg_loc,&avg_glob,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
        
        if(rank==0){
                printf("The average is : %f\n",avg_glob/size);
        }
        MPI_Finalize();
        return EXIT_SUCCESS;
}

