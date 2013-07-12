#include "./../include/gdt.h"
struct gdt_struct * gdt_st = (struct gdt_struct*)gdt;
void set_segmdesc(int num,unsigned long limit, unsigned long  base, unsigned char access , unsigned gran)
{
	
	
	gdt_st[num].base_low = base & 0xffff;
	gdt_st[num].base_middle = (base >> 16)&0xff;
	gdt_st[num].base_high = (base>>24)&0xff;
	
	gdt_st[num].limit_base = limit & 0xffff;
	gdt_st[num].limit_high = (limit >> 16)&0xf;
	gdt_st[num].limit_high |= ((gran)&0xf0);
	
	gdt_st[num].access = access;
	
	 
} 
