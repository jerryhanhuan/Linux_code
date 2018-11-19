#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>



int main(int argc,char **argv)
{
    if(argc<2)
    {
        printf("usage with num RLNUM\n");
        exit(1);
    }
    int num = atoi(argv[1]);
    int shift = atoi(argv[2]);

    int result = 0;

    //result = num >> (64-shift);
    result = num >> shift;
    printf("result::%d \n",result);
    return 0;
}


