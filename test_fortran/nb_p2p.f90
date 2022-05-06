PROGRAM nb_p2p
include "mpif.h"

PARAMETER (mat_size = 5)

INTEGER rank, size, ierror
DOUBLE PRECISION, DIMENSION(:), ALLOCATABLE :: mat
DOUBLE PRECISION sum

ALLOCATE(mat(mat_size*mat_size))
sum = 0

CALL MPI_INIT(ierror)
CALL MPI_COMM_SIZE(MPI_COMM_WORLD,size,ierror)
CALL MPI_COMM_RANK(MPI_COMM_WORLD,rank,ierror)

IF(rank == 0) THEN
 DO i=1,mat_size
  DO j=1,mat_size
  mat(j+(i-1)*mat_size) = j + (i-1)*mat_size
 ENDDO
 ENDDO
 print *,"rank",rank,"sending matrix"
 CALL MPI_SEND(mat,mat_size*mat_size,MPI_DOUBLE_PRECISION,1,123,MPI_COMM_WORLD,ierror)
 CALL MPI_RECV(sum,1,MPI_DOUBLE_PRECISION,1,123,MPI_COMM_WORLD,ierror)
 print *,"reduction de la matrice =",sum   
ELSE IF(rank == 1) THEN
 print *,"rank",rank,"receiving matrix"
 CALL MPI_RECV(mat,mat_size*mat_size,MPI_DOUBLE_PRECISION,0,123,MPI_COMM_WORLD,ierror)
 DO i=1,mat_size
  DO j=1,mat_size
   sum = sum + mat(j+(i-1)*mat_size)
  ENDDO
 ENDDO
 CALL MPI_SEND(sum,1,MPI_DOUBLE_PRECISION,0,123,MPI_COMM_WORLD,ierror)
END IF



DEALLOCATE(mat)

CALL MPI_FINALIZE(ierror)

END PROGRAM