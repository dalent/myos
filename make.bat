del /Q .\bin\*.bin
nasm -o .\bin\loader.bin bootloader.asm

nasm -o .\bin\setup.o setup.asm
nasm -o .\bin\test.o test.asm
nasm  -f aout -o .\bin\head.o head.asm


pause