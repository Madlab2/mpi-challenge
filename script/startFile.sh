git pull
sudo chmod +x install.sh
for h in worker0 worker1 worker2 worker3 worker4 worker5 
do
echo "ubuntu@$h"
ssh ubuntu@$h 'bash -s' < install.sh
done
cd ../build
cmake .. && make
ctest -VV
mpirun -n 7 -host master-node,worker0,worker1,worker2,worker3,worker4,worker5 ./main
