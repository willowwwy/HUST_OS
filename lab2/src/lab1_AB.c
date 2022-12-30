#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//线程1-递增
void* th_fn1(void *arg) //传入void*数值 a
{
	for(int i=1;i<=1000;i++){
		printf("A:%d\n",i);
		sleep(1);
	}
}

//线程2-递减
void* th_fn2(void *arg) //传入void*数值 r
{
	for(int i=1000;i>0;i--){
		printf("B:%d\n",i);
		sleep(1);
	}
}

int main()
{
	int err;//定义错误存储
   	pthread_t  tid1,tid2;//定义线程标识符

   	//创建tid1线程
   	if(err=pthread_create(&tid1,NULL,th_fn1,NULL)){
		perror("can't create thread");
	}


   	//创建tid2线程
   	if(err=pthread_create(&tid2,NULL,th_fn2,NULL)){
		perror("can't create thread");
	}

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	return 0;
}


