#include<unistd.h>
#include<stdio.h>
int main()
{
	pid_t p1=fork();
	if(p1>0){
		printf(" Parent Pid in Parent Process：%d\n",getpid());
                printf(" Child Pid in Parent Process：%d\n",p1);
		printf(" 挂起父进程...\n");
		sleep(5*1000);
		printf(" Parent process end! \n");
	}else if(p1==0){
		while(1){
			int i=1+1;
		}
		printf(" Child Pid：%d\n",getpid());
		printf(" \n父进程未结束时，子进程的父进程进程号：%d\n",getppid());
		sleep(5*1000);
		printf(" \n父进程结束后，子进程的父进程进程号：%d\n",getppid());
		printf(" Child process end! \n");
	}
	else{
		printf("创建子进程失败");
	}
	return 0;
}
