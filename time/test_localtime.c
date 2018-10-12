#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>

/*
函数:

localtime() takes into account timezone and DST settings to return a broken-down time corresponding to the system’s local time

struct tm *localtime(const time_t *timep);

recommend:
struct tm *localtime_r(const time_t *timep, struct tm *result);

    Return NULL on error

localtime( )将参数timep指向的time_t时间信息转换成以tm结构体表示的本地时区时间(如北京时间= GMT+8小时)。

*/


int main()
{

        time_t t;
        struct tm ttime;
        t=time(NULL);
        localtime_r(&t,&ttime);
        char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        //date
        printf("%d-%d-%d\n",ttime.tm_year+1900,ttime.tm_mon+1,ttime.tm_mday);
        //time
        printf("%02d:%02d:%02d\n",ttime.tm_hour,ttime.tm_min,ttime.tm_sec);
        printf("%s\n",wday[ttime.tm_wday]);
        return 0;
}

/*
output:
2018-10-12
14:13:44
Fri
*/
