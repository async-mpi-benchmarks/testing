FC = mpif90
FFLAGS = -O3 -Wall
MPIFLAGS = --use-hwthread-cpus -np 2

run: nb_p2p

test: nb_p2p
	mpirun ${MPIFLAGS} ./$<

nb_p2p: nb_p2p.f90
	${FC} ${FFLAGS} $^ -o $@

clean:
	rm -f nb_p2p