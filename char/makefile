cc = gcc
objs=keyboard.o kb.o mouse.o time.o console.o
CFLAGS = -Wall -fstrength-reduce -fomit-frame-pointer -finline-functions \
		-nostdinc -fno-builtin -c 
out = mm.a
.c.o:
	$(cc) $(CFLAGS) -o $@ $<
all: $(objs)
	rm -f  char.a
	ar rf char.a $(objs)
	ranlib char.a
	rm *.o
keyboard.o:keyboard.c
mouse.o:mouse.c
console.o:console.c
kb.o:kb.asm
	nasm -f aout -o kb.o kb.asm
