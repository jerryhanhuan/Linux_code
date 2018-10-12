#include<unistd.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>

/*
函数:

provides a simple method of converting a time_t value into printable form.

char *ctime(const time_t *timep);
recommend:    
char *ctime_r(const time_t *timep, char *buf);
    Return NULL on error

*/


int main()
{
        time_t t;
        t = time(NULL);
        char buf[50]={0};
        ctime_r(&t,buf);//buf includes a terminating newline character and a terminating null byte
        printf("%s",buf); 
        return 0;
}

/*
output:
Fri Oct 12 14:31:27 2018
*/

