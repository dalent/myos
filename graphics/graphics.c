//图形编程的网址http://www.oocities.org/garyneal_71/OldPages/cGraphicsPalette.html
//该函数负责填充一个矩形
//http://www.brackeen.com/vga/basics.html
#include "./../include/VGA.h"
#include "./../include/system.h"
#include "./../include/io.h"
void fill_rectangle(unsigned char* vram, int xsize, char c, int srcx,int srcy, int destx, int desty)
{
	int x,y;
	for(y = srcy; y <= desty; y++)
		for(x = srcx; x <= destx; x++)
			vram[y * xsize + x] = c;
}

void copy_rectangle(unsigned char *vram, int xsize, int srcx, int srcy, int width, int height, char *block)
{
	int x, y;
	for(y = srcy; y < srcy + height; y++)
		for(x = srcx; x < srcx + width; x++)
			vram[y * xsize + x] = block[(y - srcy) * width + (x - srcx)];
}

void draw_char8(unsigned char *vram, int xsize, char color, int posx, int posy, char s)
{
	int i;
	unsigned char *p;
	char *hankaku = (char*)0x6000;
	char *font = hankaku + s * 16;
	for(i = 0; i < 16; i++)
	{
		p = &vram[(posy + i) * xsize + posx];
		if((font[i] & 0x80) != 0)
			 p[0] = color;
		if((font[i] & 0x40) != 0) p[1] = color;
		if((font[i] & 0x20) != 0) p[2] = color;
		if((font[i] & 0x10) != 0) p[3] = color;
		if((font[i] & 0x08) != 0) p[4] = color;
		if((font[i] & 0x04) != 0) p[5] = color;
		if((font[i] & 0x02) != 0) p[6] = color;
		if((font[i] & 0x01) != 0) p[7] = color;
	}
}
inline void draw_string(unsigned char* vram, int xsize, char color, int posx, int posy, char*str)
{
	for(;*str!=0;str++)
	{
		draw_char8(vram,xsize ,color,posx,posy , *str);
		posx +=8;
	}
}
inline void draw_string_print(unsigned char* vram, int xsize, char color, int posx, int posy, char*str)
{
	static int x,y;
	for(;*str!=0;str++)
	{
		draw_char8(vram, xsize, color, x, y , *str);
		x += 8;
	}
}
inline void draw_char(unsigned char *vram, int xsize, char color, int posx, int posy, char s)
{
	draw_char8(vram,xsize ,color,posx,posy , s);
}

void init_mouse_cursor(unsigned char *mouse, char bc)
{
	__asm__("cld"::);//估计是之前没有清方向寄存器,下面copy内存的时候总是出错
	char cursor[16][16] = {
		"**************..",
		"*OOOOOOOOOOO*...",
		"*OOOOOOOOOO*....",
		"*OOOOOOOOO*.....",
		"*OOOOOOOO*......",
		"*OOOOOOO*.......",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOO**OOO*.....",
		"*OOO*..*OOO*....",
		"*OO*....*OOO*...",
		"*O*......*OOO*..",
		"**........*OOO*.",
		"*..........*OOO*",
		"............*OO*",
		".............***"
	};
	int x, y;

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			
			if(cursor[y][x] == '*') {
				mouse[y * 16 + x] = VGA_BLACK;
			}
			else if(cursor[y][x] == 'O') {
				mouse[y * 16 + x] = VGA_WHITE;
			}
			else if(cursor[y][x] == '.') {
				mouse[y * 16 + x] = bc;
			}
		}
	}
}

void init_screen(unsigned char *vram, int xsize, int ysize)
{
	fill_rectangle(vram, xsize, VGA_BLUE, 0, 0, xsize - 1, ysize - 29);//上层蓝色
	fill_rectangle(vram, xsize, VGA_LIGHT_GRAY, 0, ysize - 28, xsize - 1, ysize - 28);//暗灰色
	fill_rectangle(vram, xsize, VGA_WHITE, 0, ysize - 27, xsize - 1, ysize - 27);//白色
	fill_rectangle(vram, xsize, VGA_LIGHT_GRAY, 0, ysize - 26, xsize - 1, ysize - 1);//暗灰色
	//画左边的可以拖拉的按钮
	fill_rectangle(vram, xsize, VGA_WHITE, 3, ysize - 24, 59, ysize - 24);//白色线
	fill_rectangle(vram, xsize, VGA_WHITE, 2, ysize - 24, 2,  ysize - 4);//白色线
	fill_rectangle(vram, xsize, VGA_DARK_GRAY, 3, ysize - 4, 59,  ysize - 4);//暗灰色
	fill_rectangle(vram, xsize, VGA_DARK_GRAY, 59,         ysize - 23, 59,         ysize - 5);//暗灰色
	fill_rectangle(vram, xsize, VGA_BLACK,     2,          ysize - 3,  60,         ysize - 3);//黑色线
	fill_rectangle(vram, xsize, VGA_BLACK,     60,         ysize - 24, 60,         ysize - 3);//黑色线
	//画右边的显示时间的凹陷
	fill_rectangle(vram, xsize, VGA_DARK_GRAY, xsize - 47, ysize - 24, xsize -  4, ysize - 24);
	fill_rectangle(vram, xsize, VGA_DARK_GRAY, xsize - 47, ysize - 23, xsize - 47, ysize -  4);
	fill_rectangle(vram, xsize, VGA_WHITE,     xsize - 47, ysize -  3, xsize -  4, ysize -  3);
	fill_rectangle(vram, xsize, VGA_WHITE,     xsize -  3, ysize - 24, xsize -  3, ysize -  3);

}
#define CONTROL_PORT 0x03c8
#define DATA_PORT 0x03c9
void set_palette(int start, int end, unsigned char * rgb)
{
	unsigned long i,eflags;
	load_eflags(eflags);
	cli();
	outb(start, CONTROL_PORT);
	for(i = start; i <= end; i++)
	{
		outb(rgb[0] / 4, DATA_PORT);
		outb(rgb[1] / 4, DATA_PORT);
		outb(rgb[2] / 4, DATA_PORT);
		rgb += 3;
	}
	
	restore_eflags(eflags);
	
}
void init_palette(void)//可能我们需要重新写一下调色板，因为用bochs和virtualbox表现出来的颜色不一致
{
	//我们可以重新设置调色板，通过下边的方式
	 // static unsigned char table_rgb[16 * 3] = {//我们其实只需要15个颜色 这就足够了。
		// 0x00, 0x00, 0x00,	//  0:黑色 
		// 0xff, 0x00, 0x00,	//  1:亮红
		// 0x00, 0xff, 0x00,	//  2:亮绿
		// 0xff, 0xff, 0x00,	//  3:亮黄 
		// 0x00, 0x00, 0xff,	//  4:亮青 
		// 0xff, 0x00, 0xff,	//  5:亮紫 
		// 0x00, 0xff, 0xff,	//  6:亮水 
		// 0xc6, 0xc6, 0xc6,	//  7:亮灰 
		// 0x84, 0x84, 0x84,	//  8:暗灰 
		// 0x84, 0x00, 0x00,	//  9:黯红 
		// 0x00, 0x84, 0x00,	// 10:暗绿 
		// 0x84, 0x84, 0x00,	// 11:暗黄 
		// 0x00, 0x00, 0x84,	// 12:暗青 
		// 0x84, 0x00, 0x84,	// 13:暗紫 
		// 0x00, 0x84, 0x84,	// 14:暗水 
		// 0xff, 0xff, 0xff,	// 15:白 
	// };
	// set_palette(0, 15, table_rgb);
	//本来的调色板，貌似在virtualbox 和bochs不一致，我决定主动设置下
	unsigned long i,eflags;
	load_eflags(eflags);
	cli();
	outb(8, CONTROL_PORT);
	outb(0x84 / 4, DATA_PORT);
	outb(0x84 / 4, DATA_PORT);
	outb(0x84 / 4, DATA_PORT);
	restore_eflags(eflags);
	
}







