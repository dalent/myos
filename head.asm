;�����ַ�Ǵ�0x0000000��ʼ�ģ�������32λģʽ�Ĵ���
[bits 32]
extern _main
_pg_dir:;0x0000
startup_32:

			;�޸ļĴ�������Ϣ
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
pg4:;1024*768�����ṩ������ڴ棬���޸Ľ����Ҳ���һ�´����32M���ҵ�����
times 0x1000 db 0
pg5:
times 0x1000 db 0
pg6:
times 0x1000 db 0
pg7:
times 0x1000 db 0
pg8:
times 0x1000 db 0
pg9:
times 0x1000 db 0
pg10:
times 0x1000 db 0
pg11:
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
		mov ecx,1024*4;һ��ҳĿ¼�ĸ�ҳ��
		xor eax,eax
		xor edi,edi
		cld
		rep stosd
		
		mov dword[_pg_dir],     pg0  + 7;7�Ǳ�����־���ɶ���д
		mov dword[_pg_dir + 4], pg1  + 7
		mov dword[_pg_dir + 8], pg2  + 7
		mov dword[_pg_dir +12], pg3  + 7
		mov dword[_pg_dir +16], pg4  + 7
		mov dword[_pg_dir +20], pg5  + 7
		mov dword[_pg_dir +24], pg6  + 7
		mov dword[_pg_dir +28], pg7  + 7
		mov dword[_pg_dir +32], pg8  + 7
		mov dword[_pg_dir +36], pg9  + 7
		mov dword[_pg_dir +40], pg10 + 7
		mov dword[_pg_dir +44], pg11 + 7
		
		mov edi, pg11 + 4092
		mov eax, 0x2fff007
		std
b:		stosd
		
		sub eax, 0x1000
		jge b
		
		mov eax,_pg_dir
		mov cr3,eax
		mov eax,cr0
		or  eax,0x80000001
		mov cr0,eax
		ret
section .text	
		times 4096 db 0
_sys_stack: