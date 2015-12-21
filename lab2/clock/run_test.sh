echo Compiling driver..

make

echo Adding module to kernel..

sudo insmod clock.ko

echo Testing

cat /proc/clock
