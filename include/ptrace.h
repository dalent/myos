#ifndef __PTRACE_H__
#define __PTRACE_H__
/* this struct defines the way the registers are stored on the 
   stack during a system call. */
struct pt_regs {
	unsigned short gs, __gsu;
	unsigned short fs, __fsu;
	unsigned short es, __esu;
	unsigned short ds, __dsu;
  long ebp;
  long esi;
  long edi;
  long edx;
  long ecx;
  long ebx;
  long eax;
  
  long orig_eax;
  long eip;
  unsigned short cs, __csu;
  long eflags;
  long esp;
  unsigned short ss, __ssu;
};
#endif