git pull
sudo chmod +x install.sh
for host in worker2 worker1
do
echo "ubuntu@$host"
ssh ubuntu@$host 'bash -s' < install.sh
done
cd ../build
cmake .. && make
ctest -VV
mpirun -n 3 -host master-node,worker2,worker1 ./main
