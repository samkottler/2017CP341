#! /bin/bash

echo "Running serial"
./bin/mandelbrot_serial serial.png

for file in ./bin/*
do
    echo
    echo "Running $file"
    eval $file
    eval "diff out.png serial.png"
    if [ "$?" = "0" ]
    then
	echo "Correct image"
    else
	echo "Incorrect image"
    fi
done
