#include "./../include/gdt.h"
//we can all define the struct gdt_struct and the hold here .but about linux 0.11 if define macro. so for now i don't want
//define all the function about install gdt here.
void set_segmdesc(int num,unsigned long limit, unsigned long  base, unsigned char access , unsigned gran)
{
	gdt[num].base_low = (base & 0xffff);
	gdt[num].base_middle = ((base >> 16)&0xff);
	gdt[num].base_high = ((base>>24)&0xff);
	
	gdt[num].limit_base = (limit & 0xffff);
	gdt[num].limit_high = ((limit >> 16)&0xf);
	gdt[num].limit_high |= ((gran)&0xf0);
	
	gdt[num].access_right = access;
} 
