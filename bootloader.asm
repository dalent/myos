;这个程序是boot程序，也就是IPL（init program loader），放到磁盘的第一个扇区，当但脑加电的时候
;bios会吧第一个扇区的512字节加载到0x7c00， 然后本程序负责吧内核从
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
		;因为地址可以是段寄存器左移4位 + 偏移量
start:
        mov ax, BOOTSEG
        mov ss, ax
        mov ds, ax
		mov es, ax
        ;我们把自己放到0x9000处，我这里是参照了linux内0.1版本，
		;（这个位置可以防止加载内核的覆盖问题）可以改到其他的合适的位置。
        mov ax, INITSEG
		mov es,ax
        mov cx, 256
        sub si, si;ds:si 0x7c00
        sub di, di;es:di 0x9000:0x0000
		cld
		rep  movsw;一次移植2byte
		
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
		mov bx, 0x200
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
		int 0x10;打印消息
		;我们已经写了message了，是时候加载system模块了。我们将它加载到0x10000处
		mov ax, SYSSEG
		mov es, ax
		call read_it
setup:
		jmp SETUPSEG:0
;该子程序将系统模块加载到内存地址0x10000处，并确定没有跨越64k边界。我们试图尽快的进行加载，只要可能，
;就每次加载整条磁道的数据。
sread:  dw 1+ SETUPLEN
head:	dw 0;当前磁头号
track:  dw 0;当前柱面号
read_it:
		mov ax, es
		test ax, 0xfff
die:	jnz die;64k边界
		xor bx,bx
rp_read:
		mov ax, es
		cmp ax, ENDSEG
		jb ok1_read
		ret
ok1_read:
		mov ax,[cs:sectors]
		sub ax,[cs:sread]
		mov cx,ax
		shl cx,9
		add cx,bx
		jnc ok2_read
		je ok2_read
		xor ax,ax
		sub ax,bx
		shr ax,9
ok2_read:
		call read_track
		mov cx, ax         		cmp ax,[cs:sectors]
		jne ok3_read
		mov ax,1
		sub ax,[cs:head]
		jne ok4_read
		inc [cs:track]
ok4_read:
		mov [cs:head],ax
		xor ax,ax
ok3_read:
		mov [cs:sread],ax
		shl cx,9
		add bx,cx
		jnc rp_read
		mov ax,es
		add ax,0x1000
		mov es,ax
		xor bx,bx
		jmp rp_read
read_track:
		push ax
		push bx
		push cx
		push dx
		mov dx,[cs:track]
		mov cx,[cs:sread]
		inc cx
		mov ch,dl
		mov dx,[cs:head]
		mov dh,dl
		mov dl,0;驱动器号
		and dx,0x0100
		mov ah,2;读盘
		int 0x13
		jc bad_rt
		pop dx
		pop cx
		pop bx
		pop ax
		ret
bad_rt: mov ax,0
		mov dx,0
		int 0x13
		pop dx
		pop cx
		pop bx
		pop ax
		jmp read_track
sectors:
		db 18
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
		
		
		
        
        
      
