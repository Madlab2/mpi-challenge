#!/bin/bash
sudo apt-get update
sudo apt-get -y install build-essential
sudo apt-get -y install cmake
sudo apt-get -y install openmpi-bin openmpi-doc libopenmpi-dev
git clone https://github.com/Madlab2/mpi-challenge.git
cd mpi-challenge/build
cmake .. && make (&& ctest -VV)
mpirun -np 1 ./main
