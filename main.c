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
#include "./include/sheet.h"
struct BOOT_INFO
{
	char led,vmode;
	short scrnx,scrny,reserve;
	unsigned char * vram;
	unsigned long ext_mem_k;
};
extern void mem_init(long start, long end);
extern void init_screen(unsigned char *vram, int xsize, int ysize);
extern void draw_char8(unsigned char *vram, int xsize, char color, int posx, int posy, char s);
extern void trap_init();
extern void init_keyboard(struct FIFO * fifo) ;
extern int mousedecode(struct MOUSE_DEC* mdec, unsigned char dat);
extern void init_mouse(struct FIFO * fifo, int data0);
extern void copy_rectangle(unsigned char *vram, int xsize, int srcx, int srcy, int width, int height, char *block);
extern void init_mouse_cursor8(char *mouse, char bc);
extern void fill_rectangle(unsigned char* vram, int xsize, char c, int srcx,int srcy, int destx, int desty);
extern int sprintf(char * buf, const char *fmt, ...);
extern void draw_string(unsigned char* vram, int xsize, char color, int posx, int posy, char*str);
extern void printf(const char*fmt, ...);
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
void memory_set();
struct FIFO fifo;
struct FIFO fifomouse;

void main()
{
	int buf[128];
	int bufmouse[128];
	
	//int x=0,y=0;
	char  str[20];
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
	int mx,my;
	struct SHTCTL *shtctl;
	unsigned char mousebuf[256], *buf_back;
	
	struct SHEET* sht_back, *sht_mouse;
	fifo_init(&fifo, buf, 128);
	fifo_init(&fifomouse, bufmouse, 128);
	//控制内存
	memory_set();
	//初始化各种中断
	trap_init();
	//初始化pic
	init_pic();
	//time_init();
	//开启键盘中断
	init_keyboard(&fifo);
	//开启鼠标中断
	init_mouse(&fifomouse, 256);
	//初始化界面控制器
	shtctl = shtctl_init(boot_info.vram, boot_info.scrnx, boot_info.scrny);
	if(shtctl == 0)
	{
		printf("sheet init failed!");
		for(;;);
	}	
	
	//背景图层
	sht_back = sheet_alloc(shtctl);
	buf_back = (unsigned char * )malloc(boot_info.scrnx * boot_info.scrny);
	init_screen(buf_back, boot_info.scrnx, boot_info.scrny);
	sheet_setbuf(sht_back, buf_back, boot_info.scrnx, boot_info.scrny,-1);
	sheet_slide(shtctl, sht_back, 0, 0);
	//鼠标图层
	sht_mouse = sheet_alloc(shtctl);
	init_mouse_cursor8(mousebuf, 99);
	sheet_setbuf(sht_mouse, mousebuf, 16, 16, 99);
	mx = (boot_info.scrnx - 16) / 2;
	my = (boot_info.scrny - 28 - 16) / 2;
	sheet_slide(shtctl, sht_mouse, mx, my);
	
	sheet_updown(shtctl, sht_back,  0);
	//sheet_updown(sht_win, 3);
	sheet_updown(shtctl, sht_mouse, 1);
	
	sti();
	
	for(;;)
	{
		cli();
		if((fifo_status(&fifo) +fifo_status(&fifomouse))  == 0)
		{	
		//sti();
			stihlt();
		}else 
		{
			int data;
			if(fifo_status(&fifo) != 0)
			{
				data = fifo_get(&fifo);
				sti();
				if(data <= 256)
					draw_char(boot_info.vram, boot_info.scrnx, VGA_WHITE, 0, 0, kbdus[data]);
			}
			else if(fifo_status(&fifomouse) != 0)
			{
				int data;
				data = fifo_get(&fifomouse);
				if(mousedecode(&mdec, data - 256) == 1)	
				{
					//fill_rectangle(boot_info.vram, boot_info.scrnx, VGA_BLUE, mx, my, mx + 15, my + 15);
					mx += mdec.x;
					my += mdec.y;
					mx = (mx >= 0 ? ((mx <= boot_info.scrnx - 16) ? mx:boot_info.scrnx - 16) : 0);
					my = (my >= 0 ? ((my <= boot_info.scrny - 16) ? my:boot_info.scrny - 16) : 0);
					sheet_slide(shtctl, sht_mouse, mx, my);
					
				}				
				
					
			
				
			}
		}
		
	}
	//__asm__ __volatile__("hlt");
}

void memory_set()
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
}
#include "./include/stdarg.h"
extern int vsprintf(char *buf, const char *fmt, va_list args);
 extern void draw_string_print(unsigned char* vram, int xsize, char color, int posx, int posy, char*str);
void printf(const char*fmt, ...)
{
	char buf[100];
	va_list args;

	va_start(args, fmt);
	vsprintf(buf,fmt,args);
	va_end(args);
	draw_string_print(boot_info.vram, boot_info.scrnx, VGA_WHITE, 0, 0, buf);
}
