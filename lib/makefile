cc = gcc
objs=memory.o
CFLAGS = -Wall -fstrength-reduce -fomit-frame-pointer -finline-functions \
		-nostdinc -fno-builtin -c 
out = lib.a
.c.o:
	$(cc) $(CFLAGS) -o $@ $<
all: $(objs)
	rm -f  lib.a
	ar rf lib.a $(objs)
	rm *.o
malloc.o:malloc.c