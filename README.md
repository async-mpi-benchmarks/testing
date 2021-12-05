# Discovering MPI primitives

This repository regroups small, basic MPI programs. These are aimed at learning the basics of the MPI standard (using the OpenMPI implementation) and more specifically, the asynchronous primitives in view of benchmarking them later on.

## Dependencies
This project equires you to have an MPI-able compiler (such as the `mpicc` wrapper), and a shell script that allows you to run MPI programs (such as `mpirun`. You can install both of these using the [spack](https://github.com/spack/spack) package manager.

## Building
The provided Makefile gives you the following commands:
```
# Build the sync programs
make sync

# Build the async programs
make async

# Build both
make
```
You can then remove the generated executables using the provided `make clean` command.

## Running
Run the programs with the `mpirun` script and specify the number of processes to run with using the `-n` option.
An example:
```
make async
mpirun -n 2 target/async/ping_pong
mpirun -n 4 target/async/ring
```
