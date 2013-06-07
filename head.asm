;这里地址是从0x0000000开始的，并且是32位模式的代码
[bits 32]
extern _main
global _idt;
global pg0;
global _pg_dir
global setup_paging
_pg_dir:;0x0000
startup_32:

			;修改寄存器的信息
		mov ax,0x10
		mov ds,ax
		mov es,ax
		mov fs,ax
		mov gs,ax
		mov ss,ax
		xor eax,eax
testA20:inc eax
		mov dword[0x0000],eax
		cmp eax, dword[0x100000]
		je testA20
		mov esp, _sys_stack
		call setup_idt;
		jmp after_page_tables
	
size equ $ - _pg_dir
;我们现在只是保存16M的内存，所以4个页表 ，4*1024* 4k=16M就可以了
;但是我们的界面模式是1024*768，我在我的机器上测试界面的地址是0xe0000000，所我
;得额外的增加一个页面的映射来保存0xe0000000的地址
section .text
times 0x1000 - size db 0
pg0:
times 0x1000 db 0
pg1:;0x2000
times 0x1000 db 0
pg2:;0x3000
times 0x1000 db 0
pg3:;;

times 0x1000 db 0
pg4:;我想保存0xe0000000开始的4M的字节这个是vga的地址
times 0x1000 db 0


times 1024 db 0
setup_idt:
		mov edx,ignore_int
		mov  eax,0x80000
		mov  ax,dx
		mov dx,0x8e
		mov edi,_idt
		mov ecx,256
rp_sidt:
		mov [edi],eax
		mov [edi+4],edx
		add edi,8
		dec ecx
		jne rp_sidt
		lidt [idt_descr]
		ret
		

after_page_tables:


		push 0
		push 0
		push 0
		;假设main函数返回的话会从栈里面取的这个地址，进入死循环
		push L6
		;我们通过ret，来跳转到main函数
		push _main
		
		jmp setup_paging
		
L6:
		jmp L6
alignb 4
ignore_int:;我们什么也不做
		pushad
		popad
		ret
		
		
alignb 4
setup_paging:
		mov ecx,1024*4;一个页目录，我们在写五个页表
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
		
		mov eax,_pg_dir;页目录基址
		mov cr3,eax
		mov eax,cr0
		or  eax,0x80000000;开启分页
		mov cr0,eax
		ret
alignb 4
db 0,0
idt_descr:
	dw 256*8-1
	dd _idt
alignb 8
_idt:
		times 256 * 8 db 0 ;256项每项8byte
section .text	
		times 4096 db 0
_sys_stack:
