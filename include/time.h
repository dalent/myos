#ifndef _TIME_H_
#define _TIME_H_
typedef unsigned int size_t;
typedef long clock_t;
#define CLOCKS_PER_SEC 100 //系统滴答频率100HZ
struct tm
{	
	int tm_sec;//秒数
	int tm_min;//分数
	int tm_hour;//小时数
	int tm_mday;//一个月的天数
	int tm_mon;//一年中月份
	int tm_year;//从1900年开始的年数
	int tm_wday;//一星期的某天
	int tm_yday;//一年中的某天
	int tm_isdst;//夏令时标志

};
struct FIFO;
struct TIMER
{
	struct TIMER *next;  //只想下一个定时器
	unsigned long expire;//超时时间
	struct FIFO*fifo;    //超时的时候的管道
	int data;   //往管道写的数据
	unsigned char flag;   //使用状态
};
#define MAX_TIMER 255
#define TIMER_FLAGS_FREE  0//空闲状态
#define TIMER_FLAGS_ALLOC 1//正在配置状态
#define TIMER_FLAGS_USING 2//正在使用状态
struct TIMECTL
{
	unsigned long min_expire;
	struct TIMER *t0;//保存开始指针
	struct TIMER timer0[MAX_TIMER];
};
extern void init_pit();
extern void timer_free(struct TIMER*timer);
extern struct TIMER* timer_alloc();
extern void timer_settime(struct TIMER*time, unsigned long timeout);
extern void timer_init(struct TIMER*time, struct FIFO* fifo, int data);
#endif