echo "Cleaning generated files"
cd ./fifo
make clean > /dev/null 2>&1
cd ../consumer
make clean > /dev/null 2>&1
cd ../producer
make clean > /dev/null 2>&1

echo "Unloading modules"
sudo rmmod producer consumer fifo

echo "All clean!"
