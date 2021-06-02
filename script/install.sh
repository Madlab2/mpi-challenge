#!/bin/bash
sudo apt-get update
REQUIRED_PKG="build-essential"
PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $REQUIRED_PKG|grep "install ok installed")
echo Checking for $REQUIRED_PKG: $PKG_OK
if [ "" = "$PKG_OK" ]; then
  echo "No $REQUIRED_PKG. Setting up $REQUIRED_PKG."
  sudo apt-get --yes install $REQUIRED_PKG 
fi
REQUIRED_PKG="cmake"
PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $REQUIRED_PKG|grep "install ok installed")
echo Checking for $REQUIRED_PKG: $PKG_OK
if [ "" = "$PKG_OK" ]; then
  echo "No $REQUIRED_PKG. Setting up $REQUIRED_PKG."
  sudo apt-get --yes install $REQUIRED_PKG 
fi
REQUIRED_PKG="openmpi-bin"
PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $REQUIRED_PKG|grep "install ok installed")
echo Checking for $REQUIRED_PKG: $PKG_OK
if [ "" = "$PKG_OK" ]; then
  echo "No $REQUIRED_PKG. Setting up $REQUIRED_PKG."
  sudo apt-get --yes install openmpi-bin openmpi-doc libopenmpi-dev 
fi

DIRECTORY = /home/ubuntu/mpi-challenge
if [ -d $DIRECTORY ]; then
echco "_________________________Gefunden_________________________________________--"
cd mpi-challenge/build
git pull
else
echso "_________________________Muss kopiert werden_______________________________"
git clone https://github.com/Madlab2/mpi-challenge.git
cd mpi-challenge/build
fi
git checkout dev
cmake .. && make 
ctest -VV
