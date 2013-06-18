
cc = gcc
objs=graphics.o sheet.o
CFLAGS = -Wall -fstrength-reduce -fomit-frame-pointer -finline-functions \
		-nostdinc -fno-builtin -c 
out = graphics.a
.c.o:  
	$(cc) $(CFLAGS) -o $@ $<
all: $(objs)
	rm -f  $(out)
	ar rf $(out) $(objs)
	ranlib $(out)
	rm *.o
graphics.o:graphics.c
sheet.o:sheet.c
