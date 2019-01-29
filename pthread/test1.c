#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct {

    char c;
    int count;
}char_parameter;

void* char_print(void* arg)
{
    char_parameter *p = (char_parameter*)arg;
    int i = 0;
    for(i=0;i<p->count;i++)
        fputc(p->c,stderr);
    return NULL;
}

int main()
{
    pthread_t t1;
    pthread_t t2;
    int ret = 0;
    char_parameter p1;
    char_parameter p2;
    p1.c='o';
    p1.count=10;
    
    p2.c = 'x';
    p2.count = 20;

    ret = pthread_create(&t1,NULL,&char_print,&p1);
    if(ret)
    {
        perror("pthread_create1 failed");
        return -1;
    }

    ret = pthread_create(&t2,NULL,&char_print,&p2);
    if(ret)
    {
        perror("pthread_create1 failed");
        return -1;
    }

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    printf("main thread exit\n");
    return 0;
}




