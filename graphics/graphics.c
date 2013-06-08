//图形编程的网址http://www.oocities.org/garyneal_71/OldPages/cGraphicsPalette.html
//该函数负责填充一个矩形
//http://www.brackeen.com/vga/basics.html
#include "./../include/VGA.h"
void fill_rectangle(char* vram, int xsize, char c, int srcx,int srcy, int destx, int desty)
{
	int x,y;
	for(y = srcy; y <= desty; y++)
		for(x = srcx; x <= destx; x++)
			vram[y * xsize + x] = c;
}


void init_screen(char *vram, int xsize, int ysize)
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
	//画右边的显示时间的按钮
	fill_rectangle(vram, xsize, VGA_DARK_GRAY, xsize - 47, ysize - 24, xsize -  4, ysize - 24);
	fill_rectangle(vram, xsize, VGA_DARK_GRAY, xsize - 47, ysize - 23, xsize - 47, ysize -  4);
	fill_rectangle(vram, xsize, VGA_WHITE,     xsize - 47, ysize -  3, xsize -  4, ysize -  3);
	fill_rectangle(vram, xsize, VGA_WHITE,     xsize -  3, ysize - 24, xsize -  3, ysize -  3);
}



