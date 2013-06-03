set bin_path=windows_bin
set mm=.\mm

gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o %bin_path%\m.o main.c 
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o %bin_path%\t.o trap.c 
cd %mm%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o me.o memory.c
copy *.o .\..\%bin_path%\
del *.o
cd ..\%bin_path%
del /Q *.bin
nasm -o bootloader.bin ..\bootloader.asm

nasm -o setup.bin ..\setup.asm
nasm  -f aout -o head.o ..\head.asm
nasm  -f aout -o asm.o ..\asm.asm

ld -T ..\link.ld -Map map.txt -o kernel head.o m.o me.o asm.o t.o
del /Q *.o


pause