#! /bin/bash

echo "Running serial"
srun ./bin/mandelbrot_serial serial.png

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
		eval "srun -N$i -n$i $file"
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
	    eval "srun $file"
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
