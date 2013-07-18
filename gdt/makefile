cc = gcc
objs=gdt.o
CFLAGS = -Wall -fstrength-reduce -fomit-frame-pointer -finline-functions \
		-nostdinc -fno-builtin -c 
out = mm.a
.c.o:
	$(cc) $(CFLAGS) -o $@ $<
all: $(objs)
	rm -f  gdt.a
	ar rf gdt.a $(objs)
	ranlib gdt.a
	rm *.o
gdt.o:gdt.c
