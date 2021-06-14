#!/bin/bash
git pull
sudo chmod +x install.sh
for h in {0..6}
do
echo "ubuntu@worker$h"
ssh ubuntu@worker$h 'bash -s' < install.sh &> /dev/null
echo "This worker took $SECONDS seconds"
done
cd ../build
cmake .. && make
ctest -VV
echo "This script took $SECONDS seconds to execute"
mpirun -n 16 -host master-node,worker0,worker1,worker2,worker3,worker4,worker5,worker6,worker7,worker8,worker9,worker10,worker11,worker12,worker13,worker14 ./main
