#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>

/*
函数：

The gmtime() function converts a calendar time into a broken-down time corresponding to UTC


struct tm *gmtime(const time_t *timep);

recommend:
struct tm *gmtime_r(const time_t *timep, struct tm *result);
    Return NULL on error



将参数timep指向的time_t时间信息转换成以tm结构体表示的GMT时间信息
GMT是中央时区,北京在东8区,相差8个小时，所以北京时间=GMT时间+8小时
*/


int main()
{
        time_t t;
        struct tm ttime;
        t=time(NULL);
        gmtime_r(&t,&ttime);
        char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        //date
        printf("%d-%d-%d\n",ttime.tm_year+1900,ttime.tm_mon+1,ttime.tm_mday);
        //time 注意时间+8 转为北京时间
        printf("%02d:%02d:%02d\n",ttime.tm_hour+8,ttime.tm_min,ttime.tm_sec);
        //
        printf("%s\n",wday[ttime.tm_wday]);
        return 0;

}

/*
out put
2018-10-12
14:04:18
Fri
*/
