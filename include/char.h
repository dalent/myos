#ifndef _CHAR_H_
#define _CHAR_H_
#define wait_keyboard() do{\
	if((inb(0x64) & 0x2) == 0)\
		{\
			break;\
		}	\
}while(1);
extern void PIC_sendEOI(unsigned char irq);
extern void IRQ_Set_mask(unsigned char IRQline);
extern void IRQ_Clear_mask(unsigned char IRQline);

extern void do_mouse_interupt();

struct MOUSE_DEC
{
	unsigned char mouse_dbuf[3],mouse_phase; 
	int x,y,btn;
	
};
struct FIFO;
extern int mousedecode(struct MOUSE_DEC* mdec, unsigned char dat);
extern void init_mouse(struct FIFO * fifo, int data0, struct MOUSE_DEC* dec);
extern void init_keyboard(struct FIFO * fifo,unsigned int data) ;
#endif