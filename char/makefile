cc = gcc
objs=keyboard.o kb.o
CFLAGS = -Wall -fstrength-reduce -fomit-frame-pointer -finline-functions \
		-nostdinc -fno-builtin -c 
out = mm.a
.c.o:
	$(cc) $(CFLAGS) -o $@ $<
all: $(objs)
	rm -f  char.a
	ar rf char.a $(objs)
	rm *.o
keyboard.o:keyboard.c
kb.o:kb.asm
	nasm -f aout -o kb.o kb.asm
