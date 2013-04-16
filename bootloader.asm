;这个程序是boot程序，也就是IPL（init program loader），放到磁盘的第一个扇区，当但脑加电的时候
;bios会吧第一个扇区的512字节加载到0x7c0， 然后本程序负责吧内核从
;磁盘加载到内存。
SYSSIZE  EQU 0x3000;
CYLS     EQU 10 ;加载的扇区数 
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
        mov ax, BOOTSEG
        mov ss, ax
        mov ds, ax
        ;我们把自己放到0x9000处，我这里是参照了linux内0.1版本，估计是linux把内存合理的布局了。
        mov ax, INITSEG
        mov cx, 256
        sub di, di
        sub si, si
        rep
        movsw;一次移植2byte
        
        jmp INITSEG:go
        ;从下面开始我们到0x9000处执行
go:       
        mov ax, cs
        mov ds, ax
        mov es, ax
        mov ss, ax
        mov sp, 0xff00;任意设定只要不覆盖代码即可
        
        
      
