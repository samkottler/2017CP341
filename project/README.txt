Coefficient algorithm adapted from http://fraktal.republika.pl/mset_jungreis.html

Note:
None of the programs will compile on the headnode currently because the compiler on the headnode
has a problem with floating point literals. They compile fine on the compute nodes and run fine
anywhere.
MPI version will only run with one or two processes. Also, it works best on nodes 2 and 3

Modules needed on mcscn:
gcc-7.2.0
libpng-1.6.32
make-4.2
slurm
openmpi-3.0.0

Basic compilation:
srun make

To compile with debug flag:
srun make DEBUG=1

To compile with array sparseness check:
srun make CHECK_ARR=1

To run:
output is optional, default is out.png
./bin/mandelbrot_<name> [output]

mpi:
srun -N2 -n2 -x node[0-1] ./bin/mpi

test script:
./run.sh
