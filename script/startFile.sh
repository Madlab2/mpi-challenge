git pull
sudo chmod +x install.sh
Host = " worker2 worker1"
for host in $Host
do
ssh ubuntu@$host 'bash -s' < install.sh
done
cd ../build
cmake .. && make
ctest -VV
mpirun -n 3 -host master-node, worker2, worker1 ./main