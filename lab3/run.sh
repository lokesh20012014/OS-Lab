#!/bin/bash
echo "Compiling consumer.."

cd ./consumer/
make > /dev/null 2>&1

echo "Compiling producer.."
cd ../producer/
make > /dev/null 2>&1

echo "Loading Fifo kernel module.."
cd ..
sudo insmod fifo/fifo.ko

echo "Enter producer params"
echo "Production rate?"
read prate
echo "Production item?"
read pitem

echo "Loading Producer kernel module.."
sudo insmod producer/producer.ko rate=$prate item=$pitem

echo "Enter consumer params"
echo "Consumption rate?"
read crate
sudo insmod consumer/consumer.ko rate=$crate

echo "All done!"

