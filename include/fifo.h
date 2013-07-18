#ifndef _CTYPE_H_
#define _CTYPE_H_
struct TASK;
struct FIFO{
	int *buf;
	int cur;
	int p;
	int free;
	int size;
	struct TASK* task;//我们让任务与fifo对应。
};
extern void fifo_init(struct FIFO * fifo, int * buf, int size,struct TASK* task);
extern int fifo_put(struct FIFO*fifo, int data);
extern int fifo_get(struct FIFO*fifo);
extern int fifo_status(struct FIFO*fifo);
#endif