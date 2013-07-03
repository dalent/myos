#ifdef DJGPP
#define time_interrupt_asm time_interrupt_asm
#define do_time_interrupt  do_time_interrupt 
#else
#define time_interrupt_asm _time_interrupt_asm
#define do_time_interrupt  _do_time_interrupt
#endif
#define PIT_CMD 0x43       //mode/command register(write only, a read is ignored)
#define PIT_DATA 0x40      //channel 0 data port(read write)
#include "./../include/io.h"
#include "./../include/kernel.h"
#include "./../include/head.h"
#include "./../include/system.h"
#include "./../include/time.h"
/*web :http://wiki.osdev.org/Programmable_Interval_Timer
Bits         Usage
 6 and 7      Select channel :
                 0 0 = Channel 0
                 0 1 = Channel 1
                 1 0 = Channel 2
                 1 1 = Read-back command (8254 only)
 4 and 5      Access mode :
                 0 0 = Latch count value command
                 0 1 = Access mode: lobyte only
                 1 0 = Access mode: hibyte only
                 1 1 = Access mode: lobyte/hibyte //这里表示先高字节
 1 to 3       Operating mode :
                 0 0 0 = Mode 0 (interrupt on terminal count)
                 0 0 1 = Mode 1 (hardware re-triggerable one-shot)
                 0 1 0 = Mode 2 (rate generator)
                 0 1 1 = Mode 3 (square wave generator)
                 1 0 0 = Mode 4 (software triggered strobe)
                 1 0 1 = Mode 5 (hardware triggered strobe)
                 1 1 0 = Mode 2 (rate generator, same as 010b)
                 1 1 1 = Mode 3 (square wave generator, same as 011b)
 0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
*/
extern void time_interrupt_asm();
int counter;
void do_time_interrupt()
{
	PIC_sendEOI(0);
	counter++;
	
}
#define PIT_FREQUENCY 1193182
void init_pit()
{
	set_intr_gate(0x20,time_interrupt_asm);//设置中断
	outb(0x34,PIT_CMD);
	outb((PIT_FREQUENCY+CLOCKS_PER_SEC/2)/CLOCKS_PER_SEC,PIT_DATA);
	outb(((PIT_FREQUENCY+CLOCKS_PER_SEC/2)/CLOCKS_PER_SEC)>>8,PIT_DATA);
	IRQ_Clear_mask(0);//开启中断
}