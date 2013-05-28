[bits 32]
VRAM	EQU		0x90008			; ΩÁ√Êµÿ÷∑
global  _main
_main:
		mov ecx,0xfff
		mov esi,[VRAM]
L1:	
		mov byte[ds:esi],10
		inc esi
		dec ecx
		cmp ecx,0
		jne L1
		ret

		
