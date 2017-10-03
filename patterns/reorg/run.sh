#! /bin/bash

echo "serial:"
./centroid_serial
echo "openmp:"
./centroid_openmp
echo "cilk:"
./centroid_cilk
