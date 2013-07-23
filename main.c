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
#include "./include/stdlib.h"
#include "./include/kernel.h"
#include "./include/sched.h"
#include "./include/gdt.h"

struct BOOT_INFO
{
	char led,vmode;
	short scrnx,scrny,reserve;
	unsigned char * vram;
	unsigned long ext_mem_k;
};
extern void mem_init(long start, long end);
extern void trap_init();
extern void init_pic();
void show_time(struct SHTCTL* ctl,struct SHEET*sht);

extern void update_time();
struct BOOT_INFO boot_info;
unsigned long memory_end = 0;//机器具有的内存字节数
unsigned long buffer_memory_end = 0;//高速缓冲区末端地址
unsigned long main_memory_start = 0;//主内存开始的地址
#define BCD_TO_BIN(val) ((val) = ((val)&15) + ((val)>>4)*10) 
#define CMOS_READ(addr)\
 ({ outb_p(0x80|addr, 0x70);inb_p(0x71);})
 struct tm time;
static void time_init(void)
{
	
	do{
		time.tm_sec = CMOS_READ(0);
		time.tm_min = CMOS_READ(2);
		time.tm_hour = CMOS_READ(4);
		time.tm_mday = CMOS_READ(7);
		time.tm_mon = CMOS_READ(8);
		time.tm_year = CMOS_READ(9);
	}while(time.tm_sec != CMOS_READ(0));
	
	BCD_TO_BIN(time.tm_sec);
	BCD_TO_BIN(time.tm_min);
	BCD_TO_BIN(time.tm_hour);
	BCD_TO_BIN(time.tm_mday);
	BCD_TO_BIN(time.tm_mon);
	BCD_TO_BIN(time.tm_year);
}
void memory_set();


struct SHEET* open_console(struct SHTCTL *ctl)
{
	struct SHEET *sht_cons = sheet_alloc(ctl);
	unsigned char *buf_cons = (unsigned char *)malloc(256 * 165);
	if(0 == buf_cons)
	{
		panic("open console failed");
	}
	sheet_setbuf(sht_cons, buf_cons, 256, 165, -10);
	make_window(buf_cons, 256 , 165, "console", 1);
	make_textbox(sht_cons, 8, 28, 240, 128, VGA_BLACK);
	return sht_cons;
}

struct SHEET* openwindow(struct SHTCTL *ctl)
{
	struct SHEET* sht = sheet_alloc(ctl);
	unsigned char *buf = (unsigned char * )malloc(160 * 52);
	sheet_setbuf(sht, buf,160, 52, -1);
	make_window(buf, 160, 52, "window",-1);
	return sht;
}
extern int counter ;


extern void init_console(struct  SHEET* sheet);
void main()
{
	int buf[128];
	char buf1[20];
	struct TASK* tss_a, *task_cons;
	struct FIFO fifo;
	//int x=0,y=0;
	static unsigned char kbdus[128] =
	{
		0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
	  '9', '0', '-', '=', 0,	/* Backspace */
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
	int mx,my,i;
	struct SHTCTL *shtctl;
	unsigned char mousebuf[256], *buf_back;
	int cursor_x, cursor_y;
	struct SHEET* sht_back, *sht_mouse, *sht_cons,*sht_win;
	struct TIMER* time,*time1,*time2;
	char s[10];
	fifo_init(&fifo, buf, 128,0);
	//控制内存
	memory_set();
	//初始化各种中断
	trap_init();
	//初始化pic
	init_pic();
	time_init();
	//开启键盘中断
	init_keyboard(&fifo, 256);
	//开启鼠标中断
	init_mouse(&fifo, 512, &mdec);
	//初始化界面控制器
	init_pit();
	init_palette();
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
	sheet_slide(sht_back, 0, 0);
	//鼠标图层
	sht_mouse = sheet_alloc(shtctl);
	init_mouse_cursor(mousebuf, 99);
	sheet_setbuf(sht_mouse, mousebuf, 16, 16, 99);
	mx = (boot_info.scrnx - 16) / 2;
	my = (boot_info.scrny - 28 - 16) / 2;
	sheet_slide(sht_mouse, mx, my);
	
	sht_cons = open_console(shtctl);
	sheet_slide(sht_cons, 56,  6);
	
	//timer window
	sht_win = openwindow(shtctl);
	sheet_slide(sht_win, 80,72);
	make_textbox(sht_win, 8, 28, 144, 16, VGA_WHITE);
	cursor_x = 8;
	cursor_y = 28;
	
	

	
	tss_a =task_init();
	fifo.task = tss_a;
	task_run(tss_a, 1, 0);
	
	
	init_console(sht_cons);
	
	sheet_updown(sht_back,  0);
	//sheet_updown(sht_win, 3);
	sheet_updown(sht_cons, 1);
	
	sheet_updown(sht_win, 2);
	sheet_updown(sht_mouse, 3);
	
	
	show_time(shtctl,sht_back);
	
	//该时钟为了显示时间
	time = timer_alloc();
	timer_init(time,&fifo, 0);
	timer_settime(time, 1000);//60*100);
	
	time1 = timer_alloc();
	timer_init(time1,&fifo, 1);
	timer_settime(time1, 1);
	sti();
	
	for(;;)
	{
		cli();
		if(fifo_status(&fifo)  == 0)
		{	
		//sti();
			task_sleep(tss_a);
			stihlt();
		}else 
		{
			int data;
			char s[4];
			s[3] = '\0';
			data = fifo_get(&fifo);
			sti();
			if(data == 0)//时间钟表
			{
				 timer_settime(time,60*100);	
				 update_time();
				 show_time(shtctl,sht_back);
				
			}
			if(data == 1)
			{
				timer_init(time1,&fifo,2);
				fill_rectangle(sht_win->buf, sht_win->bxsize, VGA_BLACK, cursor_x, cursor_y ,cursor_x + 8, cursor_y + 16);
				timer_settime(time1,50);
				sheet_refresh(sht_win, cursor_x, cursor_y, cursor_x + 8, cursor_y + 16);
			}

			if(data == 2)
			{
				timer_init(time1,&fifo,1);
				fill_rectangle(sht_win->buf, sht_win->bxsize, VGA_WHITE, cursor_x, cursor_y ,cursor_x + 8, cursor_y + 16);
				timer_settime(time1,50);
				sheet_refresh(sht_win, cursor_x, cursor_y, cursor_x + 8, cursor_y + 16);
			}
			else if(data == 3)
			{
				//timer_settime(time1,1);
				///sprintf(buf1,"%d",counter);
				//fill_rectangle(sht_win->buf,sht_win->bxsize,VGA_WHITE, 24, 28 ,100, 50);
				//draw_string(sht_win->buf, sht_win->bxsize, VGA_BLACK, 24, 28, buf1);
				//sheet_refresh(sht_win, 24, 28, 100, 50);
				//fill_rectangle(sht_win->buf, sht_win->bxsize, VGA_BLACK, cursor_x, cursor_y ,cursor_x + 8, cursor_y + 16);
				//sheet_refresh(sht_win, cursor_x, cursor_y, cursor_x + 8, cursor_y + 16);
				
			}
			else if(data >= 256 && data < 512)
			{
				data -= 256;
				if(data < 0x54 && kbdus[data] != 0)
				{
					fill_rectangle(sht_win->buf, sht_win->bxsize, VGA_WHITE, cursor_x, cursor_y, cursor_x + 8, cursor_y + 16);
					draw_char(sht_win->buf, sht_win->bxsize, VGA_BLACK, cursor_x, cursor_y, kbdus[data]);
					sheet_refresh(sht_win, cursor_x, cursor_y, cursor_x + 8, cursor_y + 16);
					cursor_x += 8;
				}if(data == 0x0e && cursor_x > 8)//退格键
				{
					fill_rectangle(sht_win->buf, sht_win->bxsize, VGA_WHITE, cursor_x, cursor_y, cursor_x + 8, cursor_y + 16);
					sheet_refresh(sht_win, cursor_x, cursor_y, cursor_x + 8, cursor_y + 16);
					cursor_x -= 8;
				}
			}
			else 
			{
				data -= 512;
				if(mousedecode(&mdec, data) == 1)	
				{
					//fill_rectangle(boot_info.vram, boot_info.scrnx, VGA_BLUE, mx, my, mx + 15, my + 15);
					mx += mdec.x;
					my += mdec.y;
					mx = (mx >= 0 ? ((mx <= boot_info.scrnx - 16) ? mx:boot_info.scrnx - 16) : 0);
					my = (my >= 0 ? ((my <= boot_info.scrny - 16) ? my:boot_info.scrny - 16) : 0);
					sheet_slide(sht_mouse, mx, my);
					if(mdec.mouse_dbuf[0]&0x1)
					{
						sheet_slide(sht_win,mx, my);
					}
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
void printf(const char*fmt, ...)
{
	char buf[100];
	va_list args;

	va_start(args, fmt);
	vsprintf(buf,fmt,args);
	va_end(args);
	draw_string_print(boot_info.vram, boot_info.scrnx, VGA_WHITE, 0, 0, buf);
}
void update_time()
{
	if(++time.tm_min%60 == 0)
	{
		time.tm_min = 0;
		
		if(++time.tm_hour%24 == 0)
		{
			time.tm_mday++;//我们这里就先不更新月份啦，因为更新月份考虑年份关系，后边再说
		}
	}
}
void show_time(struct SHTCTL* ctl,struct SHEET*sht)
{

	char tm[20];
	sprintf(tm,"2%03d/%02d/%02d %02d:%02d",time.tm_year,time.tm_mon, time.tm_mday,time.tm_hour,time.tm_min);
	fill_rectangle(sht->buf,sht->bxsize,VGA_LIGHT_GRAY, sht->bxsize-140, sht->bysize - 20 ,sht->bxsize - 4, sht->bysize - 4 );
	draw_string(sht->buf,sht->bxsize,VGA_BLACK, sht->bxsize-140, sht->bysize - 20, tm);
	sheet_refresh(sht,sht->bxsize-140,sht->bysize - 20, sht->bxsize, sht->bysize - 4);
}




