set bin_path=windows_bin
set mm=.\mm
set mylib=.\..\lib
set idt=.\..\idt
set vga=.\..\graphics
set char=.\..\char
set std=.\..\std

gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o %bin_path%\m.o main.c 
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o %bin_path%\t.o trap.c 
cd %mm%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o me.o memory.c
copy *.o .\..\%bin_path%\
del *.o

cd %mylib%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o malloc.o malloc.c
copy *.o .\..\%bin_path%\ 
del *.o

cd %vga%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o graphics.o graphics.c
copy *.o .\..\%bin_path%\ 
del *.o

cd %idt%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o pic.o pic.c
copy *.o .\..\%bin_path%\ 
del *.o



cd %char%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o keyboard.o keyboard.c
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o mouse.o mouse.c
nasm -f aout -o kb.o kb.asm
copy *.o .\..\%bin_path%\ 
del *.o

cd %std%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o fifo.o fifo.c
copy *.o .\..\%bin_path%\ 
del *.o

cd ..\%bin_path%
del /Q *.bin
nasm -o bootloader.bin ..\bootloader.asm

nasm -o setup.bin ..\setup.asm
nasm  -f aout -o head.o ..\head.asm
nasm  -f aout -o asm.o ..\asm.asm
set object=head.o m.o me.o asm.o t.o malloc.o graphics.o pic.o keyboard.o kb.o fifo.o mouse.o
ld -T ..\link.ld -Map map.txt -o kernel %object%
del /Q *.o


pause