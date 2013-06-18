/* MShowTec - www.mshowtec.com 
** msLinux stdarg.h ver1.0 
** 20051221 lmjx create limiao@mshowtec.com 
** 
*/ 
 
#ifndef _STDARG_H_ 
#define _STDARG_H_ 
 
typedef char *va_list; 
 
#define __va_rounded_size(TYPE) \
(((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int)) 
 
#define va_start(AP, LASTARG) \
{ \
	char *p = (char*)&(LASTARG);\
	(AP = ((char *)p + __va_rounded_size (LASTARG)));\
} 
 
#define va_end(AP) ((void)(*(AP) = 0)) 
 
#define va_arg(AP, TYPE) \
(AP += __va_rounded_size (TYPE), \
*((TYPE *) (AP - __va_rounded_size (TYPE)))) 
 
#endif 