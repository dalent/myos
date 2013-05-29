binary_src=bootloader.bin setup.bin
kernel=kernel
aout_obj=head.o main.o
lib    = mm/mm.a
dir_fold=./linux_bin
cc = gcc
nasm =nasm
CFLAGS = -Wall -fstrength-reduce -fomit-frame-pointer \
        -nostdinc -fno-builtin -c
.c.o:
	$(cc) $(CFLAGS) -o $@ $<
all:$(binary_src) $(kernel)
$(binary_src): %.bin : %.asm
	$(nasm) -o $@ $<
$(kernel):main.o head.o %(lib)
	rm -f $(dir_fold)/$@
	ld -T link.ld -o $(dir_fold)/$@ head.o main.o $(lib)
	rm -f *.o
main.o:main.c
head.o:head.asm
	nasm -f aout -o $@ $<

%(lib):
	(cd mm;make)
clean:
	cd $(dir_fold);rm -rf *.bin *.o kernel *.img

