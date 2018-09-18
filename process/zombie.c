#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>


/*
   预防僵尸进程的方法:
   1. 父进程忽略 SIGCHLD
   2. 注册SIGCHLD
   3. fork 两次
   */


void signal_child(int signo)
{
	pid_t pid;
	int status;
	while((pid = waitpid(-1,&status,WNOHANG))>0)
	{
		printf("pid[%d] exiting \n",pid);
	}
}


void SSleep(int seconds)
{
	int left = seconds;
	while(left)
	{
		left = sleep(left);
	}
}


void test2(int num)
{
	int ret = 0;
	pid_t pid;
	int i = 0;
	//signal(SIGCHLD,SIG_IGN);
	signal(SIGCHLD,signal_child);
	for(i=0;i<num;i++)
	{
		pid = fork();
		if(pid>0) //parent
		{
			SSleep(2);
			continue;

		}else if(pid == 0) //child 
		{
			printf("child process pid[%d] ppid[%d]\n",getpid(),getppid());
			printf("child process exit \n");
			exit(0);

		}else{
			printf("fork() failed errno[%d]\n",errno);
			exit(1);
		}

	}
	exit(0);
}



void test3(int num)
{

	pid_t pid;
	int i = 0;
	for(i=0;i<num;i++)
	{
		pid = fork();
		if(pid>0) //parent
		{

			pid_t child_pid;
			child_pid=waitpid(pid,NULL,0);
			printf("child process child_pid[%d] pid[%d] WNOHANG[%d]\n",child_pid,pid,WNOHANG);
			continue;

		}else if(pid == 0) //child
		{
			
			pid = fork();
			if(pid>0)
			{
				printf("child process pid[%d] grandson pid[%d] exiting\n",getpid(),pid);
				exit(0);
			}else
			{
				sleep(1);
				printf("grandson process pid[%d] ppid[%d] exiting\n",getpid(),getppid());
				exit(0);

			}

		}else{
			printf("fork() failed errno[%d]\n",errno);
			exit(1);
		}

	}


}






int main()
{

	test3(10);
	sleep(20);
	return 0;

}
















