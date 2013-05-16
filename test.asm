[bits 32]
VRAM	EQU		0x90008			; ΩÁ√Êµÿ÷∑
global  _main
_main:
		mov ecx,0xfff
		mov esi,[VRAM]
		cmp esi,0xa00000
		ja L2
L1:	
		mov byte[ds:esi],10
		inc esi
		dec ecx
		cmp ecx,0
		jne L1
		ret
L2:	
		mov byte[ds:esi + 0xf000],10
		inc esi
		dec ecx
		cmp ecx,0
		jne L2
		ret
		