all: compile
	mpirun -np 6 ./a.out
	rm a.out
compile:
	mpic++ adivina_mpi.cpp -Wall