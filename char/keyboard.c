#ifdef DJGPP
#define kb_interupt_asm kb_interupt_asm
#define do_kb_interupt  do_kb_interupt 
#else
#define kb_interupt_asm _kb_interupt_asm 
#define do_kb_interupt  _do_kb_interupt 
#endif
#include "./../include/head.h"
#include "./../include/system.h"
#include "./../include/io.h"
extern void kb_interupt_asm();
extern void IRQ_Clear_mask(unsigned char IRQline);
void draw_char(char *vram, int xsize, char color, int posx, int posy, char s);
#define IQR_KB 0x21
#define KB_DATA 0x0060 
#define KB_CMD 0x64
#define KEYCMD_WAITMODE 0x60
#define KBC_MODE 0x47
static inline void  wait_keyboard()
{
	do
	{
		if((inb(0x64) & 0x2) == 0)
		{
			break;
		}
		
	}while(1);
}
void kbd_init() 
{
	set_intr_gate(IQR_KB, kb_interupt_asm); 
	//开启pic 1 中断处理
	IRQ_Clear_mask(1);
	//向键盘控制器发送命令
	wait_keyboard();
	outb_p(KEYCMD_WAITMODE, KB_CMD);
	wait_keyboard();
	outb_p(KBC_MODE, KB_DATA);
	__asm__ __volatile__("sti"::);
	
	
	
}
void do_kb_interupt()
{
	int data;
	PIC_sendEOI(1);
	data = inb(KB_DATA);
	draw_char(0xe0000000, 1024, 15, 0,0, data);
}
