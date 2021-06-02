git pull
sudo chmod +x install.sh
host = "worker1 worker2"
for h in host
do
echo "ubuntu@$h"
ssh ubuntu@$h 'bash -s' < install.sh
done
cd ../build
cmake .. && make
ctest -VV
mpirun -n 3 -host master-node,worker2,worker1 ./main
