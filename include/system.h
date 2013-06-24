//该文件定义了设置或修改描述符/中断门等的嵌入式汇编宏
#define sti() __asm__("sti"::);
#define stihlt() __asm__("sti\n" \
		"hlt"::);
#define cli() __asm__("cli"::);
#define nop() __asm__("nop"::);
#define iret() __asm__("iret"::);
#define hlt()  __asm__("hlt"::);
#define load_eflags(x)\
__asm__ __volatile__("pushfl; popl %0":"=r"(x):/*no input*/:"memory")
#define restore_eflags(x)\
__asm__ __volatile__("pushl %0;popfl":/*no output*/:"r"(x):"memory")
//eax 组成前4个字节，前两个字节是地址的低16位，后2个字节是段寄存器的选择符
//edx组成后四个字节，第一个字节是0，第2个字节是属性，后两个字节是地址的高16位
/*

  7                           0
+---+---+---+---+---+---+---+---+
| P |  DPL  | S |    GateType   |
+---+---+---+---+---+---+---+---+
Possible IDT gate types :
0b0101	 0x5	 5	 80386 32 bit Task gate
0b0110	 0x6	 6	 80286 16-bit interrupt gate
0b0111	 0x7	 7	 80286 16-bit trap gate
0b1110	 0xE	 14	 80386 32-bit interrupt gate
0b1111	 0xF	 15	 80386 32-bit trap gate

*/
#define _set_gate(gate_addr, type, dpl, addr)\
		__asm__("movw %%dx, %%ax\n\t"\
		"movw %0, %%dx\n\t"\
		"movl  %%eax, %1\n\t"\
		"movl %%edx, %2\n\t"\
		::"i"((short)(0x8000+(dpl<<13) + (type<<8))),\
		"o"(*(char*)(gate_addr)),\
		"o"(*(4 + (char*)(gate_addr))),\
		"d"((char*)(addr)),"a"(0x80000)\
		:"3","4")
//中描述符的类型是15 特权级0		
#define set_trap_gate(n, addr) \
		_set_gate(&idt[n], 15,0,addr);
//中描述符的类型是14 特权级3	
#define set_system_gate(n, addr) \
		_set_gate(&idt[n],15,3,addr);
//中描述符的类型是14 特权级0	
#define set_intr_gate(n, addr)	\
		_set_gate(&idt[n], 14, 0, addr);
		