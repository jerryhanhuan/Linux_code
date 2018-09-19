#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include <sys/types.h>
#include <sys/wait.h>
/*
child - >paernt
fd[0] read
fd[1] write

*/
int main()
{

        pid_t pid;
        char *msg="This is a pipe example\n";
        int pipefd[2]={0};
        pipe(pipefd);
        pid = fork();

        if(pid==0) //child
        {
                printf("this is child pid[%d] ppid[%d]\n",getpid(),getppid());
                close(pipefd[0]);//close read
                write(pipefd[1],msg,strlen(msg));
                exit(0);
        }else if(pid>0){
                printf("this is parent pid[%d] child_pid[%d]\n",getpid(),pid);
                close(pipefd[1]);//close write
                char buf[1024]={0};
                read(pipefd[0],buf,sizeof(buf));
                printf("recv::%s",buf);
                pid = waitpid(pid,NULL,0);
                printf("wait pid [%d]\n",pid);
                exit(0);
        }else{
                perror("fork");
                exit(1);
        }


}


