binary_src=bootloader.bin setup.bin
kernel=kernel
aout_obj=head.o test.o
dir_fold=./linux_bin
all:$(binary_src) $(kernel)
$(binary_src): %.bin : %.asm
	nasm -o $(dir_fold)/$@ $<
$(kernel):$(aout_obj)
	cd $(dir_fold);ld -T ../link.ld -o $@ $(aout_obj)
$(aout_obj):%.o:%.asm
	nasm -f aout -o $(dir_fold)/$@ $<
clean:
	cd $(dir_fold);rm -rf *.bin *.o kernel *.img

