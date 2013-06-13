#ifndef _CTYPE_H_
#define _CTYPE_H_
struct FIFO{
	int *buf;
	int cur;
	int p;
	int free;
	int size;
};
extern void fifo_init(struct FIFO * fifo, int * buf, int size);
extern int fifo_put(struct FIFO*fifo, int data);
extern int fifo_get(struct FIFO*fifo);
extern int fifo_status(struct FIFO*fifo);
#endif