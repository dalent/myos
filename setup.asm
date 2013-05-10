INITSEG  EQU 0x9000
SETUPSEG EQU 0x9020
SYSSEG   EQU 0x1000

LEDS	EQU		0x0001			;键盘状态
VMODE	EQU		0x0002			; 真彩色位数
SCRNX	EQU		0x0004			; x分辨率
SCRNY	EQU		0x0006			; y分辨率
VRAM	EQU		0x0008			; 界面地址
VBEMODE EQU		0x105
[bits 16]		
;我们首先把界面设置为1024*768，大部分机器都支持，如果不支持我们设置为320*200
		mov ax,INITSEG
		mov ds, ax
		mov ax,0x2800
		mov es,ax;缓存地址
		mov di,0;检查是否支持vesa
		mov ax,0x4f00
		int 0x10
		cmp al,0x4f
		jne scrn320
		
		mov ax,[es:di + 4];判断版本
		cmp ax,0x200
		jb scrn320
		;下边是标准的bios测试vesa的功能，请参照http://community.osdev.info/index.php?VESA
		mov cx,VBEMODE
		mov ax,0x4f01
		int 0x10
		cmp ax,0x004f;返回0x4f表示能用
		jne scrn320
		
		cmp byte[es:di + 0x19],8;颜色是 8位
		jne scrn320
		cmp byte[es:di+0x1b],4;测试是不是调色板
		jne scrn320
		mov ax,[es:di+0x00];模式属性，bit7如果不是1就不好办（这样的话就不能加上0x400）
		and ax,0x0080
		jz scrn320
		
		mov bx, VBEMODE+0x4000;1024*768
		mov ax,0x4f02
		int 0x10
		
		mov byte[VMODE],8
		mov ax,[es:di+0x12];x分辨率
		mov word[SCRNX],ax
		mov ax,[es:di+0x14]
		mov word[SCRNY],ax
		mov eax,[es:di+0x28]
		mov dword[VRAM],eax
		jmp keyboard
		
		
		
scrn320:
		MOV		AL,0x13			; 
		MOV		AH,0x00
		INT		0x10
		MOV		BYTE [VMODE],8	
		MOV		WORD [SCRNX],320		
		MOV		WORD [SCRNY],200		
		MOV		DWORD [VRAM],0x000a0000
		


keyboard:
		MOV		ah,0x02
		INT		0x16 			; keyboard BIOS
		MOV		[LEDS],al
		
		;禁用pic中断
		MOV		al,0xff
		OUT		0x21,al
		NOP						; 
		OUT		0xa1,al
		CLI						; 禁用可屏蔽中断 
		
		;开启A20管脚,也就是开启了1M以上内存
		CALL	waitkbdout
		MOV		AL,0xd1
		OUT		0x64,AL
		CALL	waitkbdout
		MOV		AL,0xdf			; enable A20
		OUT		0x60,AL
		CALL	waitkbdout
		
		

		lidt    [idt_48 + 512];因为现在ds指向0x9000，我们的加上偏移量512
		LGDT	[GDTR0 + 512]			; 
		MOV		EAX,CR0
		AND		EAX,0x7fffffff	;禁止分页
		OR		EAX,0x00000001	; 开启保护模式
		MOV		CR0,EAX
	
		
	
move:;我们把0x10000-0x8ffff中512k的内容转移到地址的0处。
		cli;我们先禁用 中断
		cld;清 方向
		mov ax,2*8
		mov ds,ax
		mov es,ax
		mov esi,0x10000
		mov edi,0x00000
		mov ecx ,0x80000/4
		a32 rep movsd
		
		

		JMP		dword 8:0x0000000;dword必须的，表示段间转移
		
waitkbdout:
		IN		 AL,0x64
		AND		 AL,0x02
		JNZ		waitkbdout		; 
		RET
		
		ALIGNB	16
GDT0:
		RESB	8	
		DW		0x07ff,0x0000,0x9a00,0x0040	; 可执行		; 
		DW		0xffff,0x0000,0x9200,0x00c0	; 数据

		DW		0
idt_48:
		dw 0
		DD 0
GDTR0:
		DW		8*3-1
		DD		GDT0+512+0x90000

		ALIGNB	16

		
; 

		
		
		
		