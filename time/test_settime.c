#define _XOPEN_SOURCE

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

#include <time.h>

    int stime(const time_t *t);

    stime() sets the system's idea of the time and date.  
    The time, pointed to by t, is measured in seconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).  
    stime() may be executed only by the superuser.

    On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.

    int settimeofday(const struct timeval *tv, const struct timezone *tz);
    Returns 0 on success, or –1 on error


*/


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


/*
    YYYY-MM-DD HH:MM:SS -> time_t——>struct timeval ->systime
*/
int set_sys_time2(char *date)
{
    time_t t;
    struct tm t_time;
    if(strptime(date,"%Y-%m-%d %H:%M:%S",&t_time) == NULL)
    {
        perror("strptime");
         return -1;
    }
    t=mktime(&t_time);
    struct timeval time_2;
    memset(&time_2,0,sizeof(time_2));
    time_2.tv_sec = t;
    int ret = settimeofday(&time_2,NULL);
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



int main(int argc,char **argv) 
{
   
    char date[50]={0};
    strcpy(date,"2017-10-13 17:51:30");
	printf("date[%s]\n",date);
	//setenv("TZ", "GMT-8", 1);
	system("date -R");
    int ret = set_sys_time(date);
    if(ret)
        printf("set_sys_time failed \n");
    else
        printf("set_sys_time success \n");
    
	ret = set_hwclock();
	if(ret)
	{
		printf("set_hwclock failed\n");
		perror("set_hwclock ");
	}else
		printf("set_hwclock ok\n");
	
	time_t t;
	time(&t);
	printf("t::[%ld]\n",t);
    return 0;
}

