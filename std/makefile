cc = gcc
objs=fifo.o
CFLAGS = -Wall -fstrength-reduce -fomit-frame-pointer -finline-functions \
		-nostdinc -fno-builtin -c 
out = std.a
.c.o:
	$(cc) $(CFLAGS) -o $@ $<
all: $(objs)
	rm -f  $(out)
	ar rf $(out) $(objs)
	rm *.o
fifo.o:fifo.c
