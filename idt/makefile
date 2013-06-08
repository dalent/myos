cc = gcc
objs=pic.o
CFLAGS = -Wall -fstrength-reduce -fomit-frame-pointer -finline-functions \
		-nostdinc -fno-builtin -c 
out = idt.a
.c.o:
	$(cc) $(CFLAGS) -o $@ $<
all: $(objs)
	rm -f  idt.a
	ar rf idt.a $(objs)
	ranlib idt.a 
	rm *.o
pic.o:pic.c
