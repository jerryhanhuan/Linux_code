#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
/*
函数:
 convert a broken-down time to printable form
    char *asctime(const struct tm *tm);
recommend:    
    char *asctime_r(const struct tm *tm, char *buf);
    Return NULL on error

*/


int main()
{
        time_t t;
        struct tm tv;
        t=time(NULL);
        localtime_r(&t,&tv);
        char buf[50]={0};
        asctime_r(&tv,buf);
        printf("%s",buf);
        return 0;
}

/*
output:
Fri Oct 12 14:37:48 2018
*/

