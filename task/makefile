cc = gcc
objs=sched.o
CFLAGS = -Wall -fstrength-reduce -fomit-frame-pointer -finline-functions \
		-nostdinc -fno-builtin -c 
out = task.a
.c.o:
	$(cc) $(CFLAGS) -o $@ $<
all: $(objs)
	rm -f  $(out)
	ar rf $(out) $(objs)
	rm *.o
sched.o:sched.c
