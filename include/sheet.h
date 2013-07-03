#ifndef _SHEET_H_
#define _SHEET_H_
struct SHEET{
	unsigned char *buf;
	int vx0,vy0,bxsize,bysize,col_inv,height,flags;
};
#define MAX_SHEETS 64
#define PAGE_SIZE 4096
#define MAP_PAGE_SIZE 1024*768    
 //位图保存整个画面的位图，为了清楚地知道点击属于哪个位图的位置
struct SHTCTL{
	unsigned char *vram, *map;
	int xsize,ysize, top;
	struct SHEET* sheets[MAX_SHEETS];
	struct SHEET sheet0[MAX_SHEETS];//层数数组
};
extern struct SHTCTL*shtctl_init(unsigned char * vram, int xsize, int ysize);
//查找空闲的图层
extern struct SHEET* sheet_alloc(struct SHTCTL* ctl);
extern void sheet_setbuf(struct SHEET*sht, unsigned char * buf, int xsize, int ysize, int col_inv);
extern void sheet_updown(struct SHTCTL *ctl, struct SHEET* sht, int height);
extern void sheet_refresh(struct SHTCTL *ctl,struct SHEET*sht,int vx0,int vy0,int vx1, int vy1);
extern void sheet_free(struct SHEET *sht);
extern void sheet_slide(struct SHTCTL *ctl,struct SHEET* sht, int vx, int vy);
extern void fill_rectangle(unsigned char* vram, int xsize, char c, int srcx,int srcy, int destx, int desty);
extern void copy_rectangle(unsigned char *vram, int xsize, int srcx, int srcy, int width, int height, char *block);
extern void draw_string(unsigned char* vram, int xsize, char color, int posx, int posy, char*str);
extern void init_mouse_cursor(unsigned char *mouse, char bc);
extern void init_screen(unsigned char *vram, int xsize, int ysize);
extern void make_window(unsigned char *buf, int xsize, int ysize , char* title, char act);
extern void make_textbox(struct SHEET *sht, int x0, int y0, int sx, int sy, int c);
extern void draw_char(unsigned char *vram, int xsize, char color, int posx, int posy, char s);
extern void init_palette(void);
#endif
