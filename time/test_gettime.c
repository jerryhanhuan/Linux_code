#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/rtc.h> 
#include <sys/ioctl.h>


#include <time.h>
#include <sys/time.h>

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
*/

/*
int clock_gettime(clockid_t clk_id, struct timespec *tp);
    Return 0 on success, or –1 on error

clock_gettime ——>struct timespec s ——> struct tm ——>format string
*/

void nowtime_ns()
{
    struct timespec ts;
    struct tm tm_time;
    time_t t;
     printf("---------------------------struct timespec---------------------------------------\n"); 
     printf("[time(NULL)]     :     %ld\n", time(NULL)); 
    if(clock_gettime(CLOCK_MONOTONIC,&ts)<0)
    {
        printf("clock_gettime failed \n");
        perror("clock_gettime");
        return;
    }
    printf("clock_gettime : tv_sec=%ld, tv_nsec=%ld\n", ts.tv_sec, ts.tv_nsec);
    localtime_r(&(ts.tv_sec),&tm_time);
    char date_time[50]={0};
    strftime(date_time,sizeof(date_time),"%Y-%m-%d %H:%M:%S",&tm_time);
    printf("clock_gettime : date_time=%s, tv_nsec=%ld\n", date_time, ts.tv_nsec);
}

/*
    gettimeofday ——>struct timeval s ——>struct tm ——>format string

    int gettimeofday(struct timeval *tv, struct timezone *tz);
    int settimeofday(const struct timeval *tv, const struct timezone *tz);
    gettimeofday() and settimeofday() return 0 for success, or -1 for failure (in which case errno is set appropriately).
*/

void nowtime_us()
{
    struct timeval tv;
    struct tm tm_time;
    printf("---------------------------struct timeval----------------------------------------\n"); 
    printf("[time(NULL)]     :     %ld\n", time(NULL)); 
    if(gettimeofday(&tv,NULL)<0)
    {
        perror("gettimeofday");
        return;
    }
    printf("gettimeofday: tv_sec=%ld, tv_usec=%ld\n", tv.tv_sec, tv.tv_usec);
    localtime_r(&(tv.tv_sec),&tm_time);
    char date_time[50]={0};
    strftime(date_time,sizeof(date_time),"%Y-%m-%d %H:%M:%S",&tm_time);
    printf("clock_gettime : date_time=%s, tv_usec=%ld\n", date_time, tv.tv_usec);
}


int main()
{
    nowtime_ns();
    printf("\n");
    nowtime_us();
    printf("\n");
    return 0;
}