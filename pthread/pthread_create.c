#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


/*
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
RETURN VALUE
       On success, pthread_create() returns 0; on error, it returns an error number, and the contents of *thread are undefined.

*/


void* thread1 (void* arg)
{
    while(1)
    {
        fputc('x',stderr);
    }
    return;
}


int main()
{

    pthread_t ptid;
    int ret = 0;
    ret = pthread_create(&ptid,NULL,&thread1,NULL);
    if(ret)
    {
        perror("pthread_create failed\n");
        return -1;
    }
    while(1)
    {
        fputc('o',stderr);
    }
    return 0;
}







