#! /bin/bash

echo "serial:"
./recover_pins_serial $1
echo "cilk:"
./recover_pins_cilk $1
echo "openmp:"
./recover_pins_openmp $1
echo "tbb:"
./recover_pins_tbb $1
