To run
This takes 1 command line argument as the value of x

mpicc -g -Wall taylor.c -lm
mpiexec -n 4 ./a.out 22

where 22 is the value of x
