/*
    sleep() 会被信号中断

  unsigned int sleep(unsigned int seconds);
  DESCRIPTION
       sleep() makes the calling thread sleep until seconds seconds have elapsed or a signal arrives which is not ignored.  
  RETURN VALUE
       Zero if the requested time has elapsed, or the number of seconds left to sleep, if the call was interrupted by a signal handler.
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>


unsigned int SSleep(unsigned int seconds)
{
    unsigned int left = seconds;
    while(left)
    {
        left = sleep(left);
    }
    return left;
}









