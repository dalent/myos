set bin_path=windows_bin
set mm=.\mm
set mylib=.\..\lib

gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o %bin_path%\m.o main.c 
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o %bin_path%\t.o trap.c 
cd %mm%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o me.o memory.c
copy *.o .\..\%bin_path%\
del *.o
cd %mylib%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o malloc.o malloc.c
copy *.o .\..\%bin_path%\ 
cd ..\%bin_path%
del /Q *.bin
nasm -o bootloader.bin ..\bootloader.asm

nasm -o setup.bin ..\setup.asm
nasm  -f aout -o head.o ..\head.asm
nasm  -f aout -o asm.o ..\asm.asm
set object=head.o m.o me.o asm.o t.o malloc.o
ld -T ..\link.ld -Map map.txt -o kernel %object%
del /Q *.o


pause