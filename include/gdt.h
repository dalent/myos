#ifndef __GDT_H__
#define __GDT_H__
struct gdt_struct{
unsigned short limit_base, base_low;
unsigned char base_middle, access_right;
unsigned char limit_high,base_high;//limit_hight 包含一部分4 bit flags
};
#endif