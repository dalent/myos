#ifndef __SCHED_H__
#define __SCHED_H__
#include "tasks.h"
//加载tr，第一次加载表示当前的运行程序。
#define load_tr(a) \
__asm__ __volatile__("ltr %%ax"::"a"(a):"1")
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
//任务切换的功能eip 没有用 cs才是目的的段寄存器
extern void farjmp(int eip, int cs);
//下边定义任务切换控制器
struct TASK{
	int sel,flags;//sel表示该进程对应的gdt号
	struct tss_struct tss;
};

struct TASKCTL{
	int running;//当前正在运行进程的个数
	int now;//记录当前正在运行的进程
	struct TASK* tasks[NR_TASKS];
	struct TASK tasks0[NR_TASKS];
};
extern  struct TASK* task_init();
extern struct TASK* task_alloc();
extern void task_run(struct TASK* task);
extern void task_switch(void);
extern void task_sleep(struct TASK* task);
#endif