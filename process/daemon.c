#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    char *logmsg = "this is daemon\n";
    pid_t pid = fork();
    if(pid>0)//parent exit
    {
        exit(0);
    }else if(pid<0)
    {
        printf("fork() failed errno[%d]\n",errno);
        exit(1);
    }
    setsid();
    chdir("/");
    umask(0);
    int i =0;
    int open_fd_num = sysconf(_SC_OPEN_MAX);
    for(i=0;i<open_fd_num;i++)
        close(i);
    while(1)
    {
        int fd = open("/var/log/test_daemon.log",O_CREAT|O_APPEND|O_WRONLY,0600);
        if(fd<0)
        {
            perror("open");
            exit(1);
        }
        write(fd,logmsg,strlen(logmsg)+1);
        close(fd);
        sleep(10);
    }
    exit(0);
}








