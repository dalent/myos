#define outb(value,port) __asm__("outb %%al, %%dx"::"a"(value),"d"(port))//硬件端口字节输出函数
#define inb(port) ({unsigned char _v; __asm__ __volatile__("inb %%dx, %%al":"=a"(_v):"d"(port);_v;})//硬件端口字节输入函数
#define outb_p(value,port)\
__asm__("outb %%al, %%dx\n"\
"\t jmp 1f\n"\
"1:\t jmp 1f\n"\
"1:"::"a"(value),"d"(port))
#define inb_p(port)({\
unsigned char _v;\
__asm__ __volatile__("inb %%dx,%%al\n"\
					"\tjmp 1f\n"\
					"1:\tjmp 1f\n\n"\
					"1:":"=a"(_v):"d"(port));\
_v;})
