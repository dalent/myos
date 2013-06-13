#define BOOT_INFO_ADDR 0x90000
#ifdef DJGPP
#define main main
#else
#define main _main
#endif
#include "./include/io.h"
#include"./include/time.h"
#include "./include/fifo.h"
#include "./include/system.h"
#include "./include/VGA.h"
#include "./include/char.h"
struct BOOT_INFO
{
	char led,vmode;
	short scrnx,scrny,reserve;
	char * vram;
	unsigned long ext_mem_k;
};
extern void mem_init(long start, long end);
void init_screen(char *vram, int xsize, int ysize);
void draw_char8(char *vram, int xsize, char color, int posx, int posy, char s);
extern void trap_init();
extern void kbd_init(struct FIFO * fifo) ;
void init_mouse(struct FIFO * fifo, int data0);
void copy_rectangle(char *vram, int xsize, int srcx, int srcy, int width, int height, char *block);
void init_mouse_cursor8(char *mouse, char bc);
struct BOOT_INFO boot_info;
unsigned long memory_end = 0;//机器具有的内存字节数
unsigned long buffer_memory_end = 0;//高速缓冲区末端地址
unsigned long main_memory_start = 0;//主内存开始的地址
#define CMOS_READ(addr)\
 ({ outb_p(0x80|addr, 0x70);inb_p(0x71);})
static void time_init(void)
{
	struct tm time;
	do{
		time.tm_sec = CMOS_READ(0);
		time.tm_min = CMOS_READ(2);
		time.tm_hour = CMOS_READ(4);
		time.tm_mday = CMOS_READ(7);
		time.tm_mon = CMOS_READ(8);
		time.tm_year = CMOS_READ(9);
	}while(time.tm_sec != CMOS_READ(0));
	
}
void init_device();
struct FIFO fifo;

void main()
{
	int buf[128];
	unsigned char kbdus[128] =
	{
		0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
	  '9', '0', '-', '=', '\b',	/* Backspace */
	  '\t',			/* Tab */
	  'q', 'w', 'e', 'r',	/* 19 */
	  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
		0,			/* 29   - Control */
	  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
	 '\'', '`',   0,		/* Left shift */
	 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
	  'm', ',', '.', '/',   0,				/* Right shift */
	  '*',
		0,	/* Alt */
	  ' ',	/* Space bar */
		0,	/* Caps lock */
		0,	/* 59 - F1 key ... > */
		0,   0,   0,   0,   0,   0,   0,   0,
		0,	/* < ... F10 */
		0,	/* 69 - Num lock*/
		0,	/* Scroll Lock */
		0,	/* Home key */
		0,	/* Up Arrow */
		0,	/* Page Up */
	  '-',
		0,	/* Left Arrow */
		0,
		0,	/* Right Arrow */
	  '+',
		0,	/* 79 - End key*/
		0,	/* Down Arrow */
		0,	/* Page Down */
		0,	/* Insert Key */
		0,	/* Delete Key */
		0,   0,   0,
		0,	/* F11 Key */
		0,	/* F12 Key */
		0,	/* All other keys are undefined */
	};
	
	struct MOUSE_DEC mdec = {0};
	
	fifo_init(&fifo, buf, 128);
	init_device();
	
	
	for(;;)
	{
		if(fifo_status(&fifo) <= 0)
		{	
			hlt();
		}else 
		{
			int data;
			cli();
			data = fifo_get(&fifo);
			sti();
			if(data <= 256)
				draw_char(boot_info.vram, boot_info.scrnx, VGA_WHITE, 0, 0, kbdus[data]);
			else
			{
				
						char mousebuf[256];
						init_mouse_cursor8(mousebuf, VGA_WHITE);
						copy_rectangle(boot_info.vram, boot_info.scrnx, 100, 100, 16, 16, mousebuf);
					
			
				
			}
		}
	}
	//__asm__ __volatile__("hlt");
}

void init_device()
{
//在这里我们的界面起始有一部分地址，这部分地址因为远超16M内存了，所以貌似完全不用考虑了。
	boot_info = *((struct BOOT_INFO*)BOOT_INFO_ADDR);
	memory_end = (unsigned long)(1 << 20) + (((unsigned long)boot_info.ext_mem_k) << 10);//内存大小=1M + 扩展内存（KB）*1024
	memory_end &= 0xfffff000;						//忽略不足4k的内存数
	if(memory_end > 16 * 1024 * 1024)				//暂时如果内存超过16M，按照16M计算
		memory_end = 16 * 1024 * 1024;
	if(memory_end > 12 * 1024 * 1024)				//如果内存 > 12M 则设置缓冲区末端=4M		
		buffer_memory_end =  4 * 1024 * 1024;
	else if(memory_end  > 6 * 1024 * 1024)          //如果大于6M则设置2M
		buffer_memory_end = 2 * 1024 * 1024;
	else											//否则设置1M	
		buffer_memory_end = 1 * 1024 * 1024; 
	
	main_memory_start = buffer_memory_end;			//主内存区起始位置=缓冲区末端
	mem_init(main_memory_start, memory_end);
	trap_init();
	init_screen(boot_info.vram, boot_info.scrnx, boot_info.scrny);
	init_pic();
	//time_init();
	kbd_init(&fifo);
	init_mouse(&fifo, 256);
}
