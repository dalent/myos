INITSEG  EQU 0x9000
SETUPSEG EQU 0x9020
SYSSEG   EQU 0x1000

LEDS	EQU		0x9001			;键盘状态
VMODE	EQU		0x9002			; 真彩色位数
SCRNX	EQU		0x9004			; x分辨率
SCRNY	EQU		0x9006			; y分辨率
VRAM	EQU		0x9008			; 界面地址
VBEMODE EQU		0x105
		
;我们首先把内界面设置为1024*768，大部分机器都支持，我们就不做错误处理了
		mov ax,INITSEG
		mov ds, ax
		mov di,0;检查是否支持VESA
		mov ax,0x2800
		mov es,ax;缓存地址
		mov ax, 0x4f00
		int 0x10
		cmp ax,0x4f;不支持的话我们跳转到300*200模式 
		jne scrn320
		
		mov ax, [es:di + 4]
		cmp ax, 0x200
		jb scrn320
		
		mov cx, VBEMODE;测试能不能使用1024*768模式
		mov ax,0x4f01
		int 0x10
		cmp ax, 0x4f
		jne scrn320
		
		cmp		byte [es:di + 0x19],8;颜色数，测试是不是8位的色彩
		jne 	scrn320
		cmp 	byte [es:di + 0x1b],4;测试是不是调色板模式
		jne 	scrn320
		mov 	ax,[es:di+0x00];模式属性，bit7如果不是1就不好办（这样的话就不能加上0x400）
		and		ax,0x0080
		jz		scrn320
		
		mov 	bx,VBEMODE+0x4000;1024*768模式
		mov 	ax,0x4f02
		int 	0x10
		
		mov 	byte[VMODE],8
		mov 	ax,[es:di+0x12];保存x分辨率
		mov 	[SCRNX],ax
		mov 	ax,[es:di+0x14]
		mov 	[SCRNY],ax
		mov 	eax,[es:di+0x28]
		mov 	[VRAM],eax
		jmp 	keystatus
scrn320:
		MOV		AL,0x13			; 
		MOV		AH,0x00
		INT		0x10
		MOV		BYTE [VMODE],8	; 
		MOV		WORD [SCRNX],320
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000

; 
keystatus:
		
		
		
		