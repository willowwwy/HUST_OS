#include <iostream>
#include <stdlib.h>
#include <string.h>

#define bsize 4	 // 物理块大小
#define psize 16 // 进程大小

class Page
{ // 页面逻辑结构
public:
	int num;  // 记录页面号
	int time; // 记录调入内存时间
};

Page b[bsize];			  // 内存单元数
int result[bsize][psize]; // 保存队列结果
int phb[bsize] = {0};	  // 物理块标号
int pro[psize] = {0};	  // 进程序列号
int missPage = 0;		  // 统计页面缺页次数

void buildNum()
{
	// 随机产生序列号函数
	printf("随机产生一个进程序列号为：\n");
	int i = 0;
	for (i = 0; i < psize; i++)
	{
		pro[i] = 10 * rand() / (RAND_MAX + 1) + 1;
		std::cout << pro[i] << " ";
	}
	std::cout << std::endl;
	return;
}

// 初始化内存单元、缓冲区
void Init(Page *b, int result[bsize][psize])
{
	for (int i = 0; i < psize; i++)
	{
		b[i].num = -1;
		b[i].time = psize - i - 1;
	}
	memset(result, -1, sizeof(int) * bsize * psize);
	memset(phb, 0, sizeof(int) * bsize);
	missPage = 0;
}

void Output()
{
	std::cout << "|---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---|" << std::endl;
	for (int j = 0; j < psize; j++)
		printf("|%2d ", pro[j]);
	std::cout << std::endl;
	std::cout << "|---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---|" << std::endl;
	for (int i = 0; i < bsize; i++)
	{
		for (int j = 0; j < psize; j++)
		{
			if (result[i][j] <= 0)
				printf("|%2c ", 32);
			else
				printf("|%2d ", result[i][j]);
		}
		std::cout << "|" << std::endl;
	}
	std::cout << "|---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---|" << std::endl;
	printf("\n缺页次数为：%6d\n缺页率：%16.6f\n", missPage, (float)(missPage) / psize);
}

void FIFO()
{
	buildNum();
	Init(b, result);
	int avbBlock = -1, samProc = -1;
	int maxWait = -1, maxProc = 0; // 标记替换物理块进程下标
	int waitTimes[bsize] = {0};	   // FIFO —— 进程等待次数(存放最久未被使用的进程标志)
	// 先进先出页面置换算法
	for (int i = 0; i < psize; i++)
	{
		avbBlock = samProc = -1;
		maxWait = -1, maxProc = 0;
		for (int j = 0; j < bsize; j++)
		{
			// 查找空闲物理块
			if (phb[j] == 0 && avbBlock == -1)
				avbBlock = j;
			// 查找相同进程
			if (phb[j] == pro[i] && samProc == -1)
				samProc = j;
			// 找waitTimes值最大的
			if (waitTimes[j] > maxProc && maxWait == -1)
			{
				maxProc = waitTimes[j];
				maxWait = j;
			}
		}

		// 不存在相同进程、存在空闲物理块
		if (samProc == -1 && avbBlock != -1)
		{
			phb[avbBlock] = pro[i]; // 进程号填入该空闲物理块
			missPage++;				// 缺页
			waitTimes[avbBlock] = 0;
			for (int j = 0; j <= avbBlock; j++)
				waitTimes[j]++;
		}
		// 不存在相同进程、不存在空闲物理块
		else if (samProc == -1 && avbBlock == -1)
		{
			phb[maxWait] = pro[i];
			waitTimes[maxWait] = 0;
			for (int j = 0; j < bsize; j++)
				waitTimes[j]++;
			missPage++; // 缺页
		}
		// 存在相同的进程
		else if (samProc != -1)
		{
			phb[samProc] = pro[i];
			for (int j = 0; j < bsize; j++)
				waitTimes[j]++;
		}
		// 记录结果
		for (int j = 0; j < bsize; j++)
			result[j][i] = phb[j];
	}

	Output();
}

void _LRU(int fold, Page *b)
{
	// LRU核心部分
	int samProc = -1;
	// 判断页面是否已在内存中
	for (int i = 0; i < bsize; i++)
	{
		if (fold == b[i].num)
			samProc = i;
	}

	// 存在相同进程
	if (samProc >= 0)
	{
		b[samProc].time = 0;
		for (int i = 0; i < bsize; i++)
			if (i != samProc)
				b[i].time++;
	}
	// 不存在相同进程
	else
	{
		// 取得在内存中停留最久的页面,默认状态下为最早调入的页面
		int Longest = -1;
		for (int i = 0; i < bsize; i++)
		{
			if (b[i].time > Longest)
			{
				Longest = b[i].time;
				samProc = i;
			}
		}
		++missPage; // 缺页
		b[samProc].num = fold;
		b[samProc].time = 0;
		for (int i = 0; i < bsize; i++)
			if (i != samProc)
				b[i].time++;
	}
}

void LRU()
{
	buildNum();
	Init(b, result);
	for (int i = 0; i < psize; i++)
	{
		_LRU(pro[i], b);

		// 记录结果
		for (int j = 0; j < bsize; j++)
			result[j][i] = b[j].num;
	}
	// 结果输出
	Output();
}

int main()
{
	int choice;
	while (1)
	{
		std::cout << "--------------------" << std::endl;
		std::cout << " 0 、 Exit	" << std::endl;
		std::cout << " 1 、 FIFO	" << std::endl;
		std::cout << " 2 、 LRU	" << std::endl;
		std::cout << "--------------------" << std::endl;
		std::cout << "选择：";
		std::cin >> choice;

		switch (choice)
		{
		case 0:
		{
			std::cout << "exit" << std::endl;
			return 0;
		}
		case 1:
		{
			std::cout << std::endl
					  << "FIFO" << std::endl;
			FIFO();
			break;
		}
		case 2:
		{
			std::cout << std::endl
					  << "LRU" << std::endl;
			LRU();
		}
		}
	}
}