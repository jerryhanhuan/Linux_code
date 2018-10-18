/*
    errno 是否是线程安全的.
    根据 man errno：
    Valid error numbers are all nonzero; errno is never set to zero by any system call or library function.
    errno is thread-local; setting it in one thread does not affect its value in any other thread.
    说明 errno 是线程安全的.
    所有的错误的errno都非零，但是不能根据errno为0与否作为判断错误的依据。大多数函数在成功的情况下并不会将errno置为0。

*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<pthread.h>

int gdata  = 0;

void * fun1()
{
    gdata = 10;
    errno = 10;
    sleep(1);
    printf("errno is %d data is %d \n",errno,gdata);
    return NULL;
}

void * fun2()
{
    gdata = 5;
    errno = 5;
    sleep(1);
    printf("errno is %d data is %d \n",errno,gdata);
    return NULL;
}


int main()
{

    pthread_t t1;
    pthread_t t2;
    int ret = 0;
    ret = pthread_create(&t1,NULL,fun1,NULL);
    if(ret)
    {
        printf("pthread_create fun1 failed errno[%d]\n",errno);
        return -1;
    }
    ret = pthread_create(&t2,NULL,fun2,NULL);
    if(ret)
    {
        printf("pthread_create fun2 failed errno[%d]\n",errno);
        return -1;
    }
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    printf("exit\n");
    return 0;
}















