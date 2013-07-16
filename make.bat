set bin_path=windows_bin
set mm=.\mm
set mylib=.\..\lib
set idt=.\..\idt
set vga=.\..\graphics
set char=.\..\char
set std=.\..\std
set gdt=.\..\gdt
set object=head.o m.o me.o asm.o t.o m2.o g.o pic.o ti.o k1.o kb.o  f.o m1.o c.o v.o s.o w.o g1.o 

gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o %bin_path%\m.o main.c 
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o %bin_path%\t.o trap.c 
cd %mm%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o me.o memory.c
copy *.o .\..\%bin_path%\
del *.o

cd %mylib%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o m2.o malloc.c
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o c.o ctype.c
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o v.o vsprintf.c
copy *.o .\..\%bin_path%\ 
del *.o

cd %vga%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o g.o graphics.c
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o s.o sheet.c
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o w.o window.c
copy *.o .\..\%bin_path%\ 
del *.o

cd %idt%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o pic.o pic.c
copy *.o .\..\%bin_path%\ 
del *.o



cd %char%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o k1.o keyboard.c
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o m1.o mouse.c
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o ti.o time.c
nasm -f aout -o kb.o kb.asm
copy *.o .\..\%bin_path%\ 
del *.o

cd %std%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o f.o fifo.c
copy *.o .\..\%bin_path%\ 
del *.o

cd %gdt%
gcc -Wall   -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin  -c -o g1.o gdt.c
copy *.o .\..\%bin_path%\ 
del *.o

cd ..\%bin_path%
del /Q *.bin
cd ..
nasm -o bootloader.bin bootloader.asm

nasm -o setup.bin setup.asm
nasm  -f aout -o head.o head.asm
nasm  -f aout -o asm.o asm.asm
copy *.o .\%bin_path%\
copy *.bin .\%bin_path%\
del /Q *.o
del /Q *.bin
cd %bin_path%
ld -T ..\link.ld -Map map.txt -o kernel %object%
del /Q *.o


pause