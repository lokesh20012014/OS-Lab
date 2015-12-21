echo Compiling driver..

make

echo Adding module to kernel..

sudo insmod fifo.ko

echo Mounting fifos 0, 1, 2, 3 ..

sudo mknod -m a+rwx /dev/fifo0 c 240 0
sudo mknod -m a+rwx /dev/fifo1 c 240 1
sudo mknod -m a+rwx /dev/fifo2 c 240 2
sudo mknod -m a+rwx /dev/fifo3 c 240 3

echo Fifos mounted
echo 0, 2 - Write-only endpoints
echo 1, 3 - Read-only endpoints
