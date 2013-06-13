#include "./../include/head.h"
#include "./../include/system.h"
#include "./../include/io.h"
#include "./../include/fifo.h"
#include "./../include/char.h"
#ifdef DJGPP
#define mouse_interupt_asm mouse_interupt_asm
#define do_mouse_interupt  do_mouse_interupt 
#else
#define mouse_interupt_asm _mouse_interupt_asm
#define do_mouse_interupt  _do_mouse_interupt
#endif
#define IQR_MOUSE 0x2c
#define PORT_KEYCMD 0x0060 
#define KB_CMD 0x64
#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE 0xf4
extern void mouse_interupt_asm();
struct FIFO * mousefifo;
int offset;
void init_mouse(struct FIFO * fifo, int data0)
{
	mousefifo = fifo;
	offset = data0;
	set_intr_gate(IQR_MOUSE, mouse_interupt_asm); 
	//IRQ_Clear_mask(0x2);//开启pic2
	//IRQ_Clear_mask(0xc);//开启鼠标中断
	outb(KEYCMD_SENDTO_MOUSE, PORT_KEYCMD);
	wait_keyboard();
	outb(MOUSECMD_ENABLE, PORT_KEYCMD);
	__asm__ __volatile__("sti"::);
}

void do_mouse_interupt()
{
	int data;
	PIC_sendEOI(12);
	data = inb(PORT_KEYCMD);
	fifo_put(mousefifo, offset + data);
}

int mousedecode(struct MOUSE_DEC* mdec, unsigned char dat)
{
		if(mdec->mouse_phase == 0)
		{
			if(dat == 0xfa)
			{
				mdec->mouse_phase = 1;
			}
			return 0;
		}else if(mdec->mouse_phase == 1)
		{
			if((dat & 0xc8) == 0x08)
			{
				mdec->mouse_dbuf[0] = dat;
				mdec->mouse_phase = 2;
			}
			return 0;
		}else if(mdec->mouse_phase == 2)
		{
			mdec->mouse_dbuf[1] = dat;
			mdec->mouse_phase = 3;
			return 0;
		}else if(mdec->mouse_phase == 3)
		{
			mdec->mouse_dbuf[2] = dat;
			mdec->mouse_phase = 1;
			mdec->btn = mdec->mouse_dbuf[0] & 0x07;
			mdec->x = mdec->mouse_dbuf[1];
			mdec->y = mdec->mouse_dbuf[2];
			if((mdec->mouse_dbuf[0] & 0x10) !=0)
			{
				mdec->x |= 0xffffff00;
			}
			if((mdec->mouse_dbuf[0] & 0x20) != 0)
			{
				mdec->y |= 0xffffff00;
			}
			mdec->y = -mdec->y;
			return 1;
		}
		return -1;
}