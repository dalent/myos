#include "./../include/fifo.h"
#define OVER_FLOW 0x1
void fifo_init(struct FIFO * fifo, int * buf, int size)//初始化
{
	fifo->buf = buf;
	fifo->size = size;
	fifo->cur = 0;
	fifo->p = 0;
	fifo->free = size;
}

int fifo_put(struct FIFO *fifo, int ch)//存放数据
{
	if(fifo->free == 0)
	{
		return -1;
	}
	
	fifo->buf[fifo->p] = ch;
	fifo->p = (++fifo->p) % fifo->size;
	--fifo->free;
	return 0;
}

int fifo_get(struct FIFO * fifo)//获得数据
{
	int data;
	if(fifo->free == fifo->size)
	{
		return -1;
	}
	
	data = fifo->buf[fifo->cur++];
	fifo->cur %= fifo->size;
	++fifo->free;
	return data;
}

int fifo_status(struct FIFO *fifo)//查看有多少数据了
{
	return fifo->size - fifo->free;
}