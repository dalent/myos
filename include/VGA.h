#ifndef _VGA_H_
#define _VGA_H_
#define VGA_BlACK         1
#define VGA_BlUE          2
#define VGA_GREEN         3
#define VGA_Cyan          4
#define VGA_Red           5
#define VGA_Magenta       6
#define VGA_Brown         7
#define VGA_Light_Gray    8
#define VGA_Dark_Gray     9
#define VGA_Light_Blue    10
#define VGA_Light_Green   11
#define VGA_Light_Cyan    12
#define VGA_Light_Red     13
#define VGA_Light_Magenta 14
#define VGA_Yellow        15
#define VGA_White         16
//vga¿ØÖÆ¶Ë¿Ú
#define PALETTE_MASK  0x3C6
#define PALETTE_READ  0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA  0x3C9
#define CONTROL_ALL   0xff
#endif