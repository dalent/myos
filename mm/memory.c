#ifndef _MEMORY_C_
#define _MEMORY_C_
#include "../include/kernel.h"
#include "../include/stdlib.h"
#define invalidate() __asm__("movl %%eax, %%cr3"::"a"(0))   //刷新叶变换高速缓存
#define LOW_MEM 0x100000									//内存低端
#define PAGING_MEMORY (15 * 1024 * 1024)                   //分页内存15M
#define PAGING_PAGES (PAGING_MEMORY >> 12)				    //分页后的物理内存页数
#define MAP_NR(addr) (((addr)-LOW_MEM) >> 12)					//指定内存地址映射为页号
#define USED 100										   //页面被占用标志
#define CODE_SPACE(addr) ((((addr) + 4095)& ~4095) < current->stat_code + current->end_code)//判断给定的地址是否位于当前进程的代码段中
unsigned long HIGH_MEMORY = 0;
#define copy_page(from, to) __asm__("cld;rep;movsl"::"S"(from),"D"(to),"c"(1024):"cx","di","si")
unsigned char mem_map[PAGING_PAGES] ={0,};
static inline volatile void oom()
{
	
}
void mem_init(unsigned long start_mem, unsigned long end_mem)
{
	int i;
	HIGH_MEMORY = end_mem;									//设置内存的最高端
	for(i = 0; i < PAGING_PAGES; i++)						//首先置所有的页面为以占用（USED = 100）状态
	{
		mem_map[i] = USED;
	}
	
	i = MAP_NR(start_mem);								//然后计算可使用起始内存的页面
	end_mem -= start_mem;									//再计算可分页处理的内存块的页面
	end_mem >>= 12;											//从而计算出可用于页面对应的页面映射数组清零	
	while(end_mem-- > 0)									//最后将这些可用页面对应的页面映射数组清零
		mem_map[i++] = 0;
}
/*
EFLAG 解释
0	CF	Carry flag	Status
1	1	Reserved	 
2	PF	Parity flag	Status
3	0	Reserved	 
4	AF	Adjust flag	Status
5	0	Reserved	 
6	ZF	Zero flag	Status
7	SF	Sign flag	Status
8	TF	Trap flag (single step)	System
9	IF	Interrupt enable flag	Control
10	DF	Direction flag	Control
11	OF	Overflow flag	Status
12-13	IOPL	I/O privilege level (286+ only), always 1 on 8086 and 186	System
14	NT	Nested task flag (286+ only), always 1 on 8086 and 186	System
15	0	Reserved, always 1 on 8086 and 186, always 0 on later models	 
*/
unsigned long get_free_page()
{
	register unsigned long __res asm("eax");
	__asm__("std;repne;scasb\n\t"                       //比较al 和[es:edi]的值设置相应的标志寄存器的值，如果df为0则增加edi，这里从后向前扫描
			"jne 1f\n\t"									//没有找到
			"movb $1,1(%%edi)\n\t"							//置1表示找到啦
			"sall $12,%%ecx\n\t"							//左移12位4K，也就是计算与最低内存的偏移量
			"addl %2,%%ecx\n\t"							   //加上最低内存也就是对应的线性地址
			"movl %%ecx,%%edx\n\t"
			"movl $1024,%%ecx\n\t"							//把此块内存初始化为0
			"leal 4092(%%edx),%%edi\n\t"                 //把eax的值存储到[es:edi]中	
			"rep;stosl\n\t"
			"movl %%edx,%%eax\n"					
			"1:"
			:"=a"(__res)
			:"0"(0),"i"(LOW_MEM),"c"(PAGING_PAGES),
			"D"(mem_map + PAGING_PAGES - 1)
			:"3","4","dx");
	return __res;//返回空闲页面
}
unsigned long get_liner_pages(int count)
{
	//cli();
	int i;
	int j;
	for(i = PAGING_PAGES - 1; i >=0; i--)
	{
		if(mem_map[i] == 0)
		{
			for(j = i; j >=0 && j > i - count; j--)
			{
				if(mem_map[j] != 0)
				{
					i = j;
					break;
				}
			}
			if(j == i - count)
			{
				for(++j;j<=i; j++)
				{
					mem_map[j] = 1;
				}
				break;
			}
		}
	}
	//sti();
	if(i == 0)
		return 0;
	return LOW_MEM + ((i - count + 1) << 12);
	
}
void free_liner_pages(unsigned long addr, int count)
{
	int i;
	if(addr < LOW_MEM) return;
	if(addr + count * 4096 >= HIGH_MEMORY) panic("trying to free noneexistent page!");
	addr -= LOW_MEM;
	addr >>= 12;
	for(i = 0; i< count; i++)
	{
		if(mem_map[addr + i]-- < 1)
			panic("trying to free free page!");
	}
}
void free_page(unsigned long addr)
{
	if(addr < LOW_MEM) return;
	if(addr >= HIGH_MEMORY)	   panic("trying to free noneexistent page!");
	addr -= LOW_MEM;
	addr >>=12;
	if(mem_map[addr]--) return;
	mem_map[addr] = 0;
	panic("trying to free free page!");
}
#endif