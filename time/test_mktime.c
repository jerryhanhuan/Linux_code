#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>

/*
函数:

The mktime() function translates a broken-down time, expressed as local time,
into a time_t value, which is returned as the function result

time_t mktime(struct tm *tm);
    Returns seconds since the Epoch corresponding to timeptr
    on success, or (time_t) –1 on error

mktime( )将参数tm指向的tm结构体数据转换成从1970年1月1日00时00分00秒至今的GMT时间经过的秒数。

*/



int main()
{

        time_t t;
        struct tm ttime;
        t = time(NULL);
        printf("t::%ld\n",t);
        //time_t ——> strcut tm
        localtime_r(&t,&ttime);

        time_t t2;
        //struct tm ——> time_t
        t2 = mktime(&ttime);
        printf("t2::%ld\n",t2);
        return 0;
}
/*
output:
t::1539325389
t2::1539325389

*/