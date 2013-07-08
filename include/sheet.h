#ifndef _SHEET_H_
#define _SHEET_H_
#define MAX_SHEETS 64
#define PAGE_SIZE 4096
#define MAP_PAGE_SIZE 1024*768  
 //位图保存整个画面的位图，为了清楚地知道点击属于哪个位图的位置
 struct SHTCTL;
 //图层信息
struct SHEET{
	unsigned char *buf;
	int vx0,vy0,bxsize,bysize,col_inv,height,flags;
	struct SHTCTL*ctl;
	
};
struct SHTCTL{
	unsigned char *vram, *map;
	int xsize,ysize, top;
	struct SHEET* sheets[MAX_SHEETS];//层数链表
	struct SHEET sheet0[MAX_SHEETS];//层数数组
};

  

extern struct SHTCTL*shtctl_init(unsigned char * vram, int xsize, int ysize);
//查找空闲的图层
extern struct SHEET* sheet_alloc(struct SHTCTL* ctl);
extern void sheet_setbuf(struct SHEET*sht, unsigned char * buf, int xsize, int ysize, int col_inv);
extern void sheet_updown( struct SHEET* sht, int height);
extern void sheet_refresh(struct SHEET*sht, int bx0, int by0, int bx1, int by1);
extern void sheet_refreshmap(struct SHTCTL*ctl, int vx0, int vy0, int vx1, int vy1, int h0);
extern void sheet_refreshsub(struct SHTCTL *ctl, int vx0, int vy0, int vx1, int vy1,int h0, int h1);
extern void sheet_free(struct SHEET *sht);
extern void sheet_slide(struct SHEET* sht, int vx, int vy);
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
