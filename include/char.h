#ifndef _CHAR_H_
#define _CHAR_H_
#define wait_keyboard() do{\
	if((inb(0x64) & 0x2) == 0)\
		{\
			break;\
		}	\
}while(1);

struct MOUSE_DEC
{
	unsigned char mouse_dbuf[3],mouse_phase; 
	int x,y,btn;
	
};
#endif