#!/bin/bash
for((i = 1; i <= 2; i += 1)); do
  mpirun -np 6 ./a.out
done