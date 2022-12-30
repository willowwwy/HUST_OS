#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
using namespace std;
#define producerNum 2 // 生产者数量
#define consumerNum 3 // 消费者数量
#define bufferSize 10 // 缓冲区大小
sem_t fullFlag;
sem_t emptyFlag;
pthread_t tid[producerNum + consumerNum];
pthread_mutex_t mutex;
int buf[bufferSize] = {0};
int ProPos = 0;
int ConPos = 0;

// 生产者进程
void *producer(void *arg)
{
    int order = *(int *)arg + 1;
    srand((unsigned int)time(NULL));
    while (true)
    {
        sem_wait(&fullFlag);
        pthread_mutex_lock(&mutex); // 上锁
        int item;
        if (order == 1)
            item = rand() % (1999 - 1000) + 1000;
        else if (order == 2)
            item = rand() % (2999 - 2000) + 2000;

        buf[ProPos] = item;
        int tmp = ProPos;
        ProPos = (ProPos + 1) % bufferSize;
        cout << "生产者: " << order << "\t放入物品: " << item << "\t在位置: " << tmp << endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&emptyFlag);
        sleep(1);
    }
    return arg;
}
// 消费者进程
void *consumer(void *arg)
{
    int order = *(int *)arg + 1;
    while (true)
    {
        sem_wait(&emptyFlag);
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < bufferSize; i++)
        {
            if (i == ConPos)
                cout << "消费者: " << order << "\t消费物品: " << buf[i] << "\t位置: " << i << endl;
        }
        sleep(4);
        ConPos = (ConPos + 1) % bufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&emptyFlag);
        sleep(1);
    }
    return arg;
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    sem_init(&fullFlag, 0, bufferSize);
    sem_init(&emptyFlag, 0, 0);
    pthread_t threadPool[producerNum + consumerNum];

    int i;
    for (i = 0; i < producerNum; i++)
    {
        pthread_t temp;
        if (pthread_create(&temp, NULL, producer, &i) == -1)
        {
            cout << "ERROR,fail to create producer " << i << endl;
            exit(1);
        }
        threadPool[i] = temp;
    } // 创建生产者进程放入线程池

    for (i = 0; i < consumerNum; i++)
    {
        pthread_t temp;
        if (pthread_create(&temp, NULL, consumer, &i) == -1)
        {
            cout << "ERROR,fail to create consumer " << i << endl;
            exit(1);
        }
        threadPool[i + producerNum] = temp;
    } // 创建消费者进程放入线程池

    void *result;
    for (i = 0; i < producerNum + consumerNum; i++)
    {
        if (pthread_join(threadPool[i], &result) == -1)
        {
            cout << "fail to recollect" << endl;
            exit(1);
        }
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&fullFlag);
    sem_destroy(&emptyFlag);

    return 0;
}
