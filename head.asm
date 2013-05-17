;这里地址是从0x0000000开始的，并且是32位模式的代码
[bits 32]
extern _main
_pg_dir:;0x0000
startup_32:

			;修改寄存器的信息
		mov ax,0x10
		mov ds,ax
		mov es,ax
		mov fs,ax
		mov gs,ax
		mov esp, _sys_stack
		jmp after_page_tables

size equ $ - _pg_dir

section .text
times 0x1000 - size db 0
pg0:
times 0x1000 db 0
pg1:;0x2000
times 0x1000 db 0
pg2:;0x3000
times 0x1000 db 0
pg3:;;0x4000
times 0x1000 db 0
pg4:;我想保存0xe0000000开始的4M的字节这个是vga的地址
times 0x1000 db 0


times 1024 db 0
		

after_page_tables:


		push 0
		push 0
		push 0
		push L6
		push _main
		
		jmp setup_paging
		
L6:
		jmp L6

		
alignb 4
setup_paging:
		mov ecx,1024*4;一个页目录四个页表
		xor eax,eax
		xor edi,edi
		cld
		rep stosd
		
		mov dword[_pg_dir],           pg0 + 7;7是保护标志，可读可写
		mov dword[_pg_dir + 4],       pg1 + 7
		mov dword[_pg_dir + 8],       pg2 + 7
		mov dword[_pg_dir +12],       pg3 + 7
		mov dword[_pg_dir + 0x380*4], pg4 + 7;

		mov edi, pg3 + 0x1000 - 4
		mov eax, 0xfff007
		std
b:		stosd
		
		sub eax, 0x1000
		jge b
		
		mov ecx, 1024;1024项
		mov edi,pg4
		mov eax,0xe0000007
		cld 
c:	    stosd
		add eax, 0x1000
		sub ecx,1
		cmp ecx,0
		ja	c
		
		mov eax,_pg_dir
		mov cr3,eax
		mov eax,cr0
		or  eax,0x80000001
		mov cr0,eax
		ret
section .text	
		times 4096 db 0
_sys_stack:
