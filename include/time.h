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
#endif