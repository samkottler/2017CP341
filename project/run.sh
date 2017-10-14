#! /bin/bash

echo "Running serial"
eval "srun -x node[0-1] ./bin/mandelbrot_serial $1 $2 serial.png"

for file in ./bin/*
do
    if [ "$file" != "./bin/mandelbrot_serial" ]
    then
	if [ "$file" = "./bin/mpi" ]
	then
	    for i in 1 2
	    do
		echo
		echo "Running mpi on $i nodes"
		eval "srun -x node[0-1] -N$i -n$i $file $1 $2"
		eval "diff out.png serial.png"
		if [ "$?" = "0" ]
		then
		    echo "Correct image"
		else
		    echo "Incorrect image"
		fi
	    done
	else
	    echo
	    echo "Running $file"
	    eval "srun -x node[0-1] $file $1 $2"
	    eval "diff out.png serial.png"
	    if [ "$?" = "0" ]
	    then
		echo "Correct image"
	    else
		echo "Incorrect image"
	    fi
	fi
    fi
done
