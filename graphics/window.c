/*
	这个文件是用来建window窗口的。
*/
#include "./../include/VGA.h"
#include "./../include/sheet.h"
#include "./../include/stdlib.h"
void write_str2window(struct SHTCTL *ctl, struct SHEET*sht, int x, int y, int c, int b, char *s, int l)//b black color,l, length
{
	fill_rectangle(sht->buf, sht->bxsize, b, x, y,  x + l * 8, y + 15);
	draw_string   (sht->buf, sht->bxsize, c, x, y, 	s);
	sheet_refresh(ctl);
}
void make_title(unsigned char * buf, int xsize, char *title, char act)
{
	char closebtn[14][16] = {
		"OOOOOOOOOOOOOOO@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQ@@QQQQ@@QQ$@",
		"OQQQQ@@QQ@@QQQ$@",
		"OQQQQQ@@@@QQQQ$@",
		"OQQQQQQ@@QQQQQ$@",
		"OQQQQQ@@@@QQQQ$@",
		"OQQQQ@@QQ@@QQQ$@",
		"OQQQ@@QQQQ@@QQ$@",
		"OQQQQQQQQQQQQQ$@",
		"OQQQQQQQQQQQQQ$@",
		"O$$$$$$$$$$$$$$@",
		"@@@@@@@@@@@@@@@@"
	};
	
	int x, y;
	char c, tc, tbc;
	if(act != 0)
	{
		tc = VGA_BLACK;
		tbc = VGA_LIGHT_CYAN;
	}else
	{
		tc = VGA_LIGHT_GRAY;
		tbc = VGA_DARK_GRAY;
	}
	
	fill_rectangle(buf, xsize, tbc, 3, 3,  xsize - 4, 20);//我们窗体的表头位置为(3,3) (xsize - 4) 20
	draw_string(   buf, xsize, tc, 24, 4, 	title);
	for(y = 0; y < 14; y++)
	{
		for(x = 0; x < 16; x++)
		{
			c = closebtn[y][x];
			if('O' == c) c = VGA_LIGHT_GRAY;
			else if('@' == c) c = VGA_BLACK;
			else if('Q' == c) c = VGA_DARK_GRAY;
			else c = VGA_BLACK;
			buf[(5 + y) * xsize + (xsize - 21 + x)] = c;//从第五行 开始
		}
	}
	
	
	
}
//画一个cmd格式的控制台
void make_window(unsigned char *buf, int xsize, int ysize , char* title, char act)//act本窗口是否处于活动状态
{
	//画上变得一条灰色线，和一条白色线条
	fill_rectangle(buf, xsize, VGA_LIGHT_GRAY, 0, 0,  xsize - 1, 0);
	fill_rectangle(buf, xsize, VGA_WHITE,     1, 1,  xsize - 2, 1);
	//画左边的一条灰色线和一天白色线
	fill_rectangle(buf, xsize, VGA_LIGHT_GRAY, 0, 0,  0,         ysize - 1);
	fill_rectangle(buf, xsize, VGA_WHITE,      1, 1,  1,         ysize - 2);
	//画下边的一条暗灰色线和一条黑色线
	fill_rectangle(buf, xsize, VGA_DARK_GRAY, 1, ysize - 2,  xsize - 2, ysize - 2);
	fill_rectangle(buf, xsize, VGA_BLACK,      0, ysize - 1,  xsize - 1, ysize - 1);
	//画右边的一条暗灰色和一条黑色
	fill_rectangle(buf, xsize, VGA_DARK_GRAY,  xsize - 2, 1,  xsize - 2,         ysize - 2);
	fill_rectangle(buf, xsize, VGA_WHITE,      xsize - 1, 0,  xsize - 1,         ysize - 1);
	//画框体里面的数据
	fill_rectangle(buf, xsize, VGA_LIGHT_GRAY, 3, 3,  xsize - 3,         ysize - 3);
	
	make_title(buf, xsize,  title, act);
}

void make_textbox(struct SHEET *sht, int x0, int y0, int sx, int sy, int c)
{
	int x1 = x0 + sx, y1 = y0 + sy;
	//画上变得一条灰色线，和一条白色线条
	fill_rectangle(sht->buf, sht->bxsize, VGA_DARK_GRAY , x0 - 2, y0 - 3,  x1 + 1, y0 - 3);
	fill_rectangle(sht->buf, sht->bxsize, VGA_DARK_GRAY , x0 - 3, y0 - 3,  x1 - 3, y1 + 1);
	fill_rectangle(sht->buf, sht->bxsize, VGA_WHITE     , x0 - 3, y1 + 2,  x1 + 1, y1 + 2);
	fill_rectangle(sht->buf, sht->bxsize, VGA_WHITE     , x1 + 2, y0 - 3,  x1 + 2, y1 + 2);
	fill_rectangle(sht->buf, sht->bxsize, VGA_BLACK     , x0 - 1, y0 - 2,  x1 + 0, y0 - 2);
	fill_rectangle(sht->buf, sht->bxsize, VGA_BLACK     , x0 - 2, y0 - 2,  x0 - 2, y1 + 0);
	fill_rectangle(sht->buf, sht->bxsize, VGA_LIGHT_GRAY, x0 - 2, y1 + 1,  x1 + 0, y1 + 1);
	fill_rectangle(sht->buf, sht->bxsize, VGA_LIGHT_GRAY, x1 + 1, y0 - 2,  x1 + 1, y1 + 1);
	fill_rectangle(sht->buf, sht->bxsize, c             , x0 - 1, y0 - 1,  x1 + 0, y1 + 0);
}
