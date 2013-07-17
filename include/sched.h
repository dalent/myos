#ifndef __SCHED_H__
#define __SCHED_H__
//加载tr，第一次加载表示当前的运行程序。
#define load_tr(a) \
__asm__ __volatile__("ltr %%ax"::"a"(32):"1")
struct tss_struct {
	unsigned short	back_link,__blh;//This is a 16-bit selector which allows linking this TSS with the previous one.
	unsigned long	esp0;
	unsigned short	ss0,__ss0h;
	unsigned long	esp1;
	unsigned short	ss1,__ss1h;
	unsigned long	esp2;
	unsigned short	ss2,__ss2h;
	unsigned long	cr3;
	unsigned long	eip;
	unsigned long	eflags;
	unsigned long	eax,ecx,edx,ebx;
	unsigned long	esp;
	unsigned long	ebp;
	unsigned long	esi;
	unsigned long	edi;
	unsigned short	es, __esh;
	unsigned short	cs, __csh;
	unsigned short	ss, __ssh;
	unsigned short	ds, __dsh;
	unsigned short	fs, __fsh;
	unsigned short	gs, __gsh;
	unsigned short	ldt, __ldth;
	unsigned short	trace, iomap;
	};
//任务切换的功能eip 没有用 cs才是目的的地址
extern farjmp(int eip, int cs);
#endif