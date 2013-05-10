del /Q .\bin\*.bin
nasm -o .\bin\loader.bin bootloader.asm

nasm -o .\bin\setup.o setup.asm
nasm -o .\bin\head.o test.asm
pause