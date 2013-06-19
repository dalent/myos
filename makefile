binary_src=bootloader.bin setup.bin
kernel=kernel
aout_obj=head.o main.o
lib    = mm/mm.a lib/lib.a graphics/graphics.a idt/idt.a char/char.a std/std.a
dir_fold=./linux_bin
cc = gcc
nasm =nasm
CFLAGS = -Wall -fstrength-reduce -fomit-frame-pointer \
        -nostdinc -fno-builtin -fno-stack-protector -c
.c.o:
	$(cc) $(CFLAGS) -o $@ $<
all:$(binary_src) $(kernel)
$(binary_src): %.bin : %.asm
	$(nasm) -o $(dir_fold)/$@ $<
$(kernel):head.o main.o asm.o trap.o graphics/graphics.a lib/lib.a idt/idt.a char/char.a std/std.a mm/mm.a
	rm -f $(dir_fold)/$@
	-ld -T link.ld -Map $(dir_fold)/map -o $(dir_fold)/$@ $^
	rm -f *.o
	rm -f $(lib)
main.o:main.c
trap.o:trap.c
head.o:head.asm
	nasm -f aout -o $@ $<
asm.o:asm.asm
	nasm -f aout -o $@ $<
mm/mm.a:
	(cd mm;make)
lib/lib.a:
	(cd lib;make)
graphics/graphics.a:
	(cd graphics;make)
idt/idt.a:
	(cd idt;make)
char/char.a:
	(cd char;make)
std/std.a:
	(cd std;make)
clean:
	cd $(dir_fold);rm -rf *.bin *.o kernel *.img

