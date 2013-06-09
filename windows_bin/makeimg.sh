echo "begin"
if [ -f "bootloader.bin" ];then
dd if=bootloader.bin of=disk.img bs=512 count=1
else
echo "bootloaer.bin not exist"
exit
fi
if [ -f "setup.bin" ]; then
dd if=setup.bin of=disk.img bs=512 seek=1
else
echo "setup.bin not exist"
exit
fi

if [ -f "kernel" ]; then
dd if=kernel of=disk.img bs=512 seek=5
else
echo "kernel not exist"
exit
fi
dd if=/dev/zero of=disk.img bs=512 seek=120 count=2760
echo "success!"


