#include <mpi.h>
#include <stdio.h>

int MPI_Send( const void* buf , int count , MPI_Datatype datatype , int dest , int tag , MPI_Comm comm)
{
    printf("Coucou de %s count : %d\n", __FUNCTION__, count);
    int ret = PMPI_Send(  buf ,  count , datatype ,  dest ,  tag ,  comm);

    /* APRES */
    //stock()

    return ret;
}

int MPI_Isend( const void* buf , int count , MPI_Datatype datatype , int dest , int tag , MPI_Comm comm , MPI_Request* request)
{
    printf("Coucou de %s count : %d\n", __FUNCTION__, count);

    return PMPI_Isend( buf , count ,  datatype ,  dest ,  tag ,  comm , request);
}

int MPI_Wait( MPI_Request* request , MPI_Status* status)
{
    printf("Coucou de %s\n", __FUNCTION__);

    /*
MPI_Request MPI_Request_f2c(MPI_Fint request)
MPI_Fint MPI_Request_c2f(MPI_Request request)
    */
    return PMPI_Wait( request , status);
}

int MPI_Recv( void* buf , int count , MPI_Datatype datatype , int dest , int tag , MPI_Comm comm, MPI_Status * st)
{
    printf("Coucou de %s count : %d\n", __FUNCTION__, count);
    return PMPI_Recv(  buf ,  count , datatype ,  dest ,  tag ,  comm, st);
}

MPI_Init( int* argc , char*** argv)
{
    /* MPI MARCHE */
    
}
// Faire correspondre un wait à un isend/irecv donné
// MPI_Request ? (hash(sizeof(MPI_Request)))




