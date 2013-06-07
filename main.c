#define BOOT_INFO_ADDR 0x90000
#ifdef DJGPP
#define main main
#else
#define main _main
#endif
struct BOOT_INFO
{
	char led,vmode;
	short scrnx,scrny,reserve;
	char * vram;
	unsigned long ext_mem_k;
};
extern void mem_init(long start, long end);
void init_screen(char *vram, int xsize, int ysize);
extern void trap_init();
struct BOOT_INFO boot_info;
unsigned long memory_end = 0;//机器具有的内存字节数
unsigned long buffer_memory_end = 0;//高速缓冲区末端地址
unsigned long main_memory_start = 0;//主内存开始的地址
void main()
{
	int i = 0;
	//在这里我们的界面起始有一部分地址，这部分地址因为远超16M内存了，所以貌似完全不用考虑了。
	boot_info = *((struct BOOT_INFO*)BOOT_INFO_ADDR);
	for(;i < boot_info.scrnx * boot_info.scrny; i++)
	{
		boot_info.vram[i] = 15;
	}
	memory_end = (unsigned long)(1 << 20) + (((unsigned long)boot_info.ext_mem_k) << 10);//内存大小=1M + 扩展内存（KB）*1024
	memory_end &= 0xfffff000;						//忽略不足4k的内存数
	if(memory_end > 16 * 1024 * 1024)				//暂时如果内存超过16M，按照16M计算
		memory_end = 16 * 1024 * 1024;
	if(memory_end > 12 * 1024 * 1024)				//如果内存 > 12M 则设置缓冲区末端=4M		
		buffer_memory_end =  4 * 1024 * 1024;
	else if(memory_end  > 6 * 1024 * 1024)          //如果大于6M则设置2M
		buffer_memory_end = 2 * 1024 * 1024;
	else											//否则设置1M	
		buffer_memory_end = 1 * 1024 * 1024; 
	
	main_memory_start = buffer_memory_end;			//主内存区起始位置=缓冲区末端
	mem_init(main_memory_start, memory_end);
	trap_init();
	init_screen(boot_info.vram, boot_info.scrnx, boot_info.scrny);
	for(;;);
}
