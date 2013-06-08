#ifndef _VGA_H_
#define _VGA_H_
#define VGA_BLACK         0//黑
#define VGA_BLUE          1//蓝
#define VGA_GREEN         2//绿
#define VGA_CYAN          3//青绿色
#define VGA_RED           4//红
#define VGA_MAGENTA       5//红紫色; 洋红
#define VGA_BROWN         6//褐色
#define VGA_LIGHT_GRAY    7//灰色
#define VGA_DARK_GRAY     8//暗灰色
#define VGA_LIGHT_BLUE    9//蓝色
#define VGA_LIGHT_GREEN   10
#define VGA_LIGHT_CYAN    11
#define VGA_LIGHT_RED     12
#define VGA_LIGHT_MAGENTA 13
#define VGA_YELLOW        14
#define VGA_WHITE         15
//vga控制端口
#define PALETTE_MASK  0x3C6
#define PALETTE_READ  0x3C7
#define PALETTE_WRITE 0x3C8
#define PALETTE_DATA  0x3C9
#define CONTROL_ALL   0xff
#endif
