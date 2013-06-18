cc = gcc
objs=malloc.o ctype.o vsprintf.o
CFLAGS = -Wall -fstrength-reduce -fomit-frame-pointer -finline-functions \
		-nostdinc -fno-builtin -c 
out = lib.a
.c.o:
	$(cc) $(CFLAGS) -o $@ $<
all: $(objs)
	rm -f  lib.a
	ar rf lib.a $(objs)
	ranlib lib.a
	rm *.o
malloc.o:malloc.c
ctype.o:ctype.c
vsprintf.o:vsprintf.c
