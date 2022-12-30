#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<stdio.h>
int main()
{
        pid_t p1=fork();
        if(p1>0){
		int child_ret;
                printf("\n Parent Pid in Parent Process：%d\n",getpid());
                printf(" Child Pid in Parent Process：%d\n",p1);
                printf("Start Waiting...\n");
                wait(&child_ret);
		printf("子进程返回参数：%d\n",child_ret);
                printf(" Parent process end! \n");
        }else if(p1==0){
                printf("\n Child Pid in Child Process：%d\n",getpid());
		printf("Start sleeping...(5s)\n");
		sleep(5);
                printf(" Child process end! \n");
		exit(0);
        }
        else{
                printf("创建子进程失败\n");
        }
        return 0;
}
