#!/bin/bash
for i in 2 4 8 12 16 ; do
	echo "$1 $i"
	mpirun -np $i --hostfile hosts $1 1000000000
done
