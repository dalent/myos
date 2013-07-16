#ifndef __GDT_H__
#define __GDT_H__
 #ifdef DJGPP
 #define gdt gdt
#else
  #define gdt _gdt
 #endif
 #include "tasks.h"
typedef struct {
unsigned short limit_base, base_low;
unsigned char base_middle, access_right;
unsigned char limit_high,base_high;//limit_hight 包含一部分4 bit flags
}gdt_struct[NR_TASKS];
extern void set_segmdesc(int num,unsigned long limit, unsigned long  base, unsigned char access , unsigned gran);
extern gdt_struct gdt ;
/*
http://wiki.osdev.org/Global_Descriptor_Table
*/
#endif