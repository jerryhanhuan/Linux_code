
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>

/*
time_t : type long int ,用来存储从1970到现在过了多少秒；

更精确一点，可以使用 struct timeval
struct timeval{
long tv_sec;    //秒
long tv_usec;   //微秒
}

struct timeval有两个成员，一个是秒，一个是微秒, 所以最高精确度是微秒。
一般由函数int gettimeofday(struct timeval *tv, struct timezone *tz)获取系统的时间 

struct timespec {
time_t tv_sec; // seconds
long tv_nsec; // and nanoseconds
};

struct timespec有两个成员，一个是秒，一个是纳秒, 所以最高精确度是纳秒。
eg:
struct timespec ts;
clock_gettime(CLOCK_MONOTONIC,&ts);

CLOCK_REALTIME 统当前时间，从1970年1.1日算起
CLOCK_MONOTONIC 系统的启动时间，不能被设置
CLOCK_PROCESS_CPUTIME_ID 本进程运行时间
CLOCK_THREAD_CPUTIME_ID 本线程运行时间

struct timezone{  
    int tz_minuteswest; //miniutes west of Greenwich* 
    int tz_dsttime; //type of DST correction
}; 


直接存储年月日的结构:
struct tm
{
int tm_sec; //秒，正常范围0-59， 但允许至61
int tm_min; //分钟，0-59
int tm_hour; //小时， 0-23
int tm_mday; //日，即一个月中的第几天，1-31
int tm_mon; //月， 从一月算起，0-11
int tm_year; //  Year - 1900 年， 从1900至今已经多少年
int tm_wday; //星期，一周中的第几天， 从星期日算起，0-6
int tm_yday; //从今年1月1日到目前的天数，范围0-365
int tm_isdst; //日光节约时间的旗标
};


需要特别注意的是，年份是从1900年起至今多少年，而不是直接存储如2008年，月份从0开始的，0表示一月，星期也是从0开始的， 0表示星期日，1表示星期一。

常用函数
#include <time.h>

//获取 Unix timestamp
time_t time(time_t *timep);
    Returns number of seconds since the Epoch,or (time_t) –1 on error
    Epoch则是指定为1970年一月一日凌晨零点零分零秒，格林威治时间

eg: time_t = time(NULL);    

struct tm *gmtime(const time_t *timep);
struct tm *localtime(const time_t *timep);
    Both return a pointer to a statically allocated broken-down
    time structure on success, or NULL on error
    The  return  value points to a statically allocated struct which might be overwritten by subsequent calls to any of the date and time functions

recommend:
 struct tm *gmtime_r(const time_t *timep, struct tm *result);
 struct tm *localtime_r(const time_t *timep, struct tm *result);


size_t strftime(char *s, size_t max, const char *format,const struct tm *tm);
    The strftime() function formats the broken-down time tm according to the format specification format and places the result in the character array s of size max
    Returns number of bytes placed in outstr (excluding terminating null byte) on success, or 0 on error

char *strptime(const char *s, const char *format, struct tm *tm);
    The strptime() function is the converse of strftime(). It converts a date-plus-time string to a broken-down time.
    Returns pointer to next unprocessed character in str on success, or NULL on error

time_t mktime(struct tm *tm);
    convert struct tm to time_t
    Returns seconds since the Epoch corresponding to timeptr on success, or (time_t) –1 on error

char *ctime(const time_t *timep);
char *ctime_r(const time_t *timep, char *buf);
    provides a simple method of converting a time_t value into printable form.
    return  NULL on error

*/
/*
    获取当前格式化日期: time_t -> YYYY-MM-DD HH:MM:SS
    流程: time_t(日历时间) ——> struct tm(以年月日、时分秒表示的时间) ——>格式化时间(strftime)
*/

int get_format_date(char *date)
{
    time_t t;
    struct tm time_1;
    char buf[50]={0};
    t = time(NULL);
    memset(&time_1,0,sizeof(time_1));
    localtime_r(&t,&time_1);
    strftime(buf,sizeof(buf),"%Y-%m-%d %H:%M:%S",&time_1);
    strcpy(date,buf);
    return 0;
}


/*
    格式化时间转为时间戳 : YYYY-MM-DD HH:MM:SS ——> time_t
    流程: 格式化时间(strptime) ——> struct tm(以年月日、时分秒表示的时间) ——>time_t(日历时间)
*/

time_t get_unixtime_date(char *date)
{
    time_t t;
    struct tm time_1;
    memset(&time_1,0,sizeof(time_1));
    if(strptime(date,"%Y-%m-%d %H:%M:%S",&time_1) == NULL)
        return -1;
    t= mktime(&time_1);
    return t;
}


/*
    获取当前可见日期: time_t ——> printable string
    流程: time_t(日历时间) ——> 可见日期
    返回格式为: Fri Oct 12 11:40:58 2018\n"

*/

int get_print_date(char *date)
{
    time_t t;
    char buf[50]={0};
    t=time(NULL);
    ctime_r(&t,buf);
    strcpy(date,buf);
    return 0;
}


/*
    获取当前可见日期: struct tm ——> printable string
    流程: struct tm ——> 可见日期
    返回格式为: Fri Oct 12 11:40:58 2018\n"

*/

int get_print_date2(char *date)
{
    time_t t;
    char buf[50]={0};
    struct tm ttime;
    time(&t);
    localtime_r(&t,&ttime);
    asctime_r(&ttime,buf);
    strcpy(date,buf);
    return 0;
}


/*
    YYYY-MM-DD HH:MM:SS ——> time_t——> systime
    
*/

int set_sys_time(char *date)
{
    int ret = 0;
    time_t t;
    struct tm t_time;
    if(strptime(date,"%Y-%m-%d %H:%M:%S",&t_time) == NULL)
    {
        perror("strptime");
         return -1;
    }
       
    t = mktime(&t_time);
	printf("set t::[%ld]\n",t);
    ret = stime(&t);
    return ret;
}


int set_hwclock()
{
    static char *rtc_device = "/dev/rtc";
    int n = 50; //try time
    int rtc ; //dev fd
    while(( rtc = open(rtc_device,O_WRONLY,0666))<0)
    {
            //	"/dev/rtc" are exclusive open device
            if(!(--n) || errno!=EBUSY)
            {
                return errno;
            }
            usleep(20*1000); // 20 ms delay
    }
    time_t t;
    struct tm utc;
    time(&t);
	printf("get t::[%ld]\n",t);
    gmtime_r(&t,&utc);
    utc.tm_isdst = 0;	// disable DST
    if (ioctl(rtc,RTC_SET_TIME, (struct rtc_time *)&utc) == -1)
	{
		n = errno;
		close(rtc);
		return n;
	}	
	close(rtc);
	return 0;	
}












int main()
{

        char date[50]={0};
        get_format_date(date);
        printf("date is %s \n",date);
        time_t t;
        t = get_unixtime_date(date);
        printf("unix timestamp::[%ld]\n",t);

        memset(date,0,sizeof(date));
        get_print_date(date);
        printf("date::%s",date);
        memset(date,0,sizeof(date));
        get_print_date2(date);
        printf("date::%s",date);
        printf("done\n");
        return 0;


}


