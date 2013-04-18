;这个程序是boot程序，也就是IPL（init program loader），放到磁盘的第一个扇区，当但脑加电的时候
;bios会吧第一个扇区的512字节加载到0x7c0， 然后本程序负责吧内核从
;磁盘加载到内存。
[bits 16]
SYSSIZE  EQU 0x3000;
CYLS     EQU 10 ;加载的扇区数 
SETUPLEN EQU 4
BOOTSEG  EQU 0x7c0
INITSEG  EQU 0x9000
SETUPSEG EQU 0x9020
SYSSEG   EQU 0x1000
ENDSEG   EQU SYSSEG + SYSSIZE

entry:
        jmp start;3字节的跳转指令
        ;标准的磁盘属性
		DB		0x90
		DB		"HARIBOTE"		; 
		DW		512				; 
		DB		1				; 
		DW		1				; 
		DB		2				; 
		DW		224				; 
		DW		2880			; 
		DB		0xf0			; 
		DW		9				; 
		DW		18				; 
		DW		2				; 
		DD		0				; 
		DD		2880			; 
		DB		0,0,0x29		; 
		DD		0xffffffff		; 
		DB		"HARIBOTEOS "	; 
		DB		"FAT12   "		; 
		
        ;设置各个段寄存器的值使他指向0x7c0
start:
        mov ax, BOOTSEG
        mov ss, ax
        mov ds, ax
		mov es, ax
        ;我们把自己放到0x9000处，我这里是参照了linux内0.1版本，
		;（这个位置可以防止加载内核的覆盖问题）可以改到其他的合适的位置。
        mov ax, INITSEG
        mov cx, 256
        sub di, di
        sub si, si
		cld
		a32 rep  movsw;一次移植2byte
		mov ah, 0x03
		xor bh,bh
		int 0x10;读取光标位置
		
		mov cx,24
		mov bx,0x7
		mov bp,msg1
		mov ax,0x1301
		int 0x10
		
        jmp INITSEG:go
        ;从下面开始我们到0x9000处执行
go:       
        mov ax, cs
        mov ds, ax
        mov es, ax
        mov ss, ax
        mov sp, 0xff00;任意设定只要不覆盖代码即可
		;下面我们加载setup模块。从第二个扇区到第六个扇区。
load_setup:	
		mov ax, 0x200 + SETUPLEN;ah = 0x20读磁盘， al=4 读扇区数量
		mov cx, 0x0002	        ;CH柱面号，CL扇区号
		mov dx, 0x0000		    ;DH磁头号，DL驱动器号
		int 0x13				;ES：BX缓冲区地址
		jnc ok_load_setup		;如果错误CF位置位，jump if not carry
		mov dx, 0x0000
		mov ax, 0x0000
		jmp load_setup
ok_load_setup:
		;去磁盘驱动器参数，例如每道的扇区数量，用int 0x13中断，ah为0x08，dl驱动器号。
		;返回ah=0，al=0；bl=驱动器类型；ch最大磁道号的低8位，cl每磁道的扇区号（0-5位）
		;最大磁道号的高2位(6-7位），dh=最大磁头数，dl=驱动器的数量，es：di软驱参数表
		;若出错CF置位ah状态码
		mov dl, 0x00
		mov ax, 0x800
		int 0x13
		mov ch, 0x00
		mov ax, sectors
		mov word [cs:sectors], cx
		mov ax, INITSEG
		mov es, ax
		;打印消息
		mov ah, 0x03
		xor bh,bh
		int 0x10;读取光标位置
		
		mov cx,24
		mov bx,0x7
		mov bp,msg1
		mov ax,0x1301
		int 0x10
hang:
		jmp hang
sectors:
		dw 0
msg1:
		db 13,10;回车键
		db "Loading system ..."
		db 13,10,13,10
size equ $ - entry
%if size +2 > 512
		%error "code is too large for boot sector"
%endif
		times (512 - size -2) db 2
		db 0x55,0xAA;2字节的磁盘标识
		
		
		
        
        
      
