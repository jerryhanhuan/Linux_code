/*
note:
父进程结束或者异常终止，而它的一个或多个子进程还在运行，这些子进程被称为孤儿进程
孤儿进程被init进程(pid = 1)接管，并由init进程对它们完成状态收集工作;
孤儿进程被init进程接管，而init进程会循环的wait()它的已退出的子进程，因此，孤儿进程并不会有什么危害;
*/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
int main()
{
        pid_t pid;
        pid = fork();
        if(pid>0)//parent
        {
                printf("this is parent process,pid[%d] child_pid[%d]\n",getpid(),pid);
                //保证子进程输出父进程的ID
                sleep(1);
                printf("parent process exit\n");
                exit(0);

        }else if(pid == 0)//child
        {
                printf("this is child process,pid[%d] ppid[%d]\n",getpid(),getppid());
                //等待父进程先退出
                sleep(3);
                printf("after parent process exit,the child pid[%d] new parent pid[%d]\n",getpid(),getppid());
                printf("the child process exit \n");
                exit(0);

        }else{
                printf("fork() failed errno[%d]\n",errno);
                exit(1);
        }
}


