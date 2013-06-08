#ifndef _TIME_H_
#define _TIME_H_
typedef unsigned int size_t;
typedef long clock_t;
#define CLOCKS_PER_SEC 100 //ÏµÍ³µÎ´ğÆµÂÊ100HZ
struct tm
{	
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;

};
#endif