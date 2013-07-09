//参照网址
//http://wiki.osdev.org/Mouse_Input#Waiting_to_Send_Bytes_to_Port_0x60_and_0x64
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
#define PORT_KEYCMD 0x0064 
#define KB_DATA 0x0060
#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE 0xf4
extern void mouse_interupt_asm();
struct FIFO * mousefifo;
static unsigned int offset;
void init_mouse(struct FIFO * fifo, int data0, struct MOUSE_DEC* dec)
{
	mousefifo = fifo;
	offset = data0;
	set_intr_gate(IQR_MOUSE, mouse_interupt_asm); 
	IRQ_Clear_mask(0x2);//开启pic2
	IRQ_Clear_mask(0xc);//开启鼠标中断
	//IRQ_Clear_mask(1);
	wait_keyboard();
	outb(KEYCMD_SENDTO_MOUSE, PORT_KEYCMD);//告诉控制器我们需要操作鼠标
	wait_keyboard();
	outb(MOUSECMD_ENABLE, KB_DATA);//
	wait_keyboard();//如果成功的话0xfa会被送过来
	dec->mouse_phase = 0;
}

void do_mouse_interupt()
{
	unsigned int data;
	PIC_sendEOI(12);
	
	data = inb(KB_DATA);
	fifo_put(mousefifo, offset + data);
}
/*
			Bit 7	 Bit 6	     Bit 5	     Bit 4	    Bit 3	    Bit 2	     Bit 1	 Bit 0
Byte 1	 Y overflow	 X overflow	 Y sign bit	 X sign bit	 Always 1	 Middle Btn	 Right Btn	 Left Btn
Byte 2	 X movement
Byte 3	 Y movement
*/
int mousedecode(struct MOUSE_DEC* mdec, unsigned char dat)
{
		if(mdec->mouse_phase == 0)
		{
			if(dat == 0xfa)//鼠标回应任何命令或者数据之前都会发送，第一个阶段就是等待0xfa的阶段。
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