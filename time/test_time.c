#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
/*
函数:

 double difftime(time_t time1, time_t time0);

 #define difftime(t1,t0) (double)(t1 - t0)


#include <sys/time.h>

int gettimeofday(struct timeval *tv, struct timezone *tz);

int settimeofday(const struct timeval *tv, const struct timezone *tz);

    gettimeofday() and settimeofday() return 0 for success, or -1 for failure (in which case errno is set appropriately).

    The use of the timezone structure is obsolete; the tz argument should normally be specified as NULL


 struct timeval {
               time_t      tv_sec;     // seconds 
               suseconds_t tv_usec;    // microseconds 
           };

*/



int main()
{
        time_t t1;
        time_t t2;
        time(&t1);
        sleep(2);
        time(&t2);
        printf("t1:%ld\n",t1);
        printf("t2:%ld\n",t2);
        double diff = difftime(t2,t1);
        printf("diff:%f\n",diff);

        struct timeval tv;
        gettimeofday(&tv,NULL);
        printf("sec:%ld\n",tv.tv_sec);

        return 0;
}

/*
output:
t1:1539327040
t2:1539327042
diff:2.000000
*/