cc = gcc
objs=memory.o
CFLAGS = -Wall -fstrength-reduce -fomit-frame-pointer -finline-functions \
		-nostdinc -fno-builtin -c 
out = mm.a
.c.o:
	$(cc) $(CFLAGS) -o $@ $<
all: $(objs)
	rm -f  mm.a
	ar rf mm.a $(objs)
	rm *.o
memory.o:memory.c
