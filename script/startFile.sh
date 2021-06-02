#!/bin/bash
git pull
sudo chmod +x install.sh
for h in {0..5}
do
echo "ubuntu@$h"
ssh ubuntu@worker$h 'bash -s' < install.sh
done
cd ../build
cmake .. && make
ctest -VV
mpirun -n 7 -host master-node,worker0,worker1,worker2,worker3,worker4,worker5 ./main
