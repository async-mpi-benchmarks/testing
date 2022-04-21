FC = mpif90
FFLAGS = -O3 -Wall
MPIFLAGS = --use-hwthread-cpus -np 4

run: test_hw_f

exe: test_hw_f
	mpirun ${MPIFLAGS} ./$<

test_hw_f: test_hw_f.f90
	${FC} ${FFLAGS} $^ -o $@

clean:
	rm -f main