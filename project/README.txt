Note:
None of the programs will compile on the headnode currently because the compiler on the headnode
has a problem with floating point literals. They compile fine on the compute nodes and run fine
anywhere.

Modules needed on mcscn:
gcc-7.2.0
libpng-1.6.32
make-4.2
slurm

Basic compilation:
srun make

To compile with debug flag:
srun make DEBUG=1

To compile with array sparseness check:
srun make CHECK_ARR=1

To run:
./bin/mandelbrot_serial
./bin/mandelbrot_cilk
./bin/mandelbrot_openmp
