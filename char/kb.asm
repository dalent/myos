[bits 32]
global _kb_interupt_asm;int 0x21
global _mouse_interupt_asm;int 0x21
extern _do_kb_interupt
extern _do_mouse_interupt
_kb_interupt_asm:
		pushad
		push fs
		push es
		push ds
		mov ax,ss
		mov fs,ax
		mov es,ax
		mov ds,ax
		mov eax,_do_kb_interupt
		call eax
		pop ds
		pop es
		pop fs
		popad
		iretd
_mouse_interupt_asm:
		pushad
		push fs
		push es
		push ds
		mov ax,ss
		mov fs,ax
		mov es,ax
		mov ds,ax
		mov eax,_do_mouse_interupt
		call eax
		pop ds
		pop es
		pop fs
		popad
		iretd
		