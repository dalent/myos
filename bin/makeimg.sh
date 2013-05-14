echo "begin"
if [ -f "bootloader.bin" ];then
dd if=bootloader.bin of=disk.img bs=512 count=1
else
echo "bootloaer.bin not exist"
fi

