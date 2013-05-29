#define invalidate() __asm__("movl %%eax, %%cr3"::"a"(0))   //刷新叶变换高速缓存
#define LOW_MEM 0x100000									//内存低端
#define PAGING_MEMORY (15 * 1024 * 1024)                   //分页内存15M
#define PAGING_PAGES (PAGING_MEMORY >> 12)				    //分页后的物理内存页数
#define MAP_NR(addr) (((addr)-LOW_MEM) >> 12)					//指定内存地址映射为页号
#define USED 100										   //页面被占用标志
#define CODE_SPACE(addr) ((((addr) + 4095)& ~4095) < current->stat_code + current->end_code)//判断给定的地址是否位于当前进程的代码段中
static long HIGH_MEMORY = 0;
#define copy_page(from, to) __asm__("cld;rep;movsl"::"S"(from),"D"(to),"c"(1024):"cx","di","si")
static unsigned char mem_map[PAGING_PAGES] ={0,};
void mem_init(long start_mem, long end_mem)
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