[bits 32]
VRAM	EQU		0x90008			; ΩÁ√Êµÿ÷∑
		mov cx,0xffff
		mov esi,dword[0x90008]
hah:	
		mov byte[ds:esi],10
		inc esi
		dec cx
		cmp cx,0
		jne hah
		