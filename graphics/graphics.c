//图形编程的网址http://www.oocities.org/garyneal_71/OldPages/cGraphicsPalette.html
//该函数负责填充一个矩形
#include "../include/vga.h"
void fill_rectangle(char* vram, int xsize, char c, int srcx,int srcy, int destx, int desty)
{
	int x,y;
	for(y = srcy; y < desty; y++)
		for(x = srcx; x < destx; x++)
			vram[y * xsize + x] = c;
}


void init_screen(char *vram, int xsize, int ysize)
{
	fill_rectangle(vram, xsize, VGA_BlUE, 0, 0, xsize - 1, ysize - 29);
}



