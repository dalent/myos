del /Q .\bin\*.bin
nasm -o .\bin\bootloader.bin bootloader.asm

nasm -o .\bin\setup.bin setup.asm
nasm  -f aout -o .\bin\test.o test.asm
nasm  -f aout -o .\bin\head.o head.asm
ld -T link.ld  -o .\bin\kernel .\bin\head.o .\bin\test.o 
rem del /Q .\bin\*.o


pause