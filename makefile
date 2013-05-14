binary_src=bootloader.bin setup.bin
aout_obj=head.o
all:$(binary_src) $(aout_obj)
$(binary_src): %.bin : %.asm
	nasm -o ./bin/$@ $<
$(aout_obj):%.o:%.asm
	nasm -f aout -o ./bin/$@ $<
clean:
	rm -rf ./bin/*.bin ./bin/*.o

