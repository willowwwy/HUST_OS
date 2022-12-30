#include <iostream>
#include <stdlib.h>
#include <string.h>

#define bsize 4	 // ������С
#define psize 16 // ���̴�С

class Page
{ // ҳ���߼��ṹ
public:
	int num;  // ��¼ҳ���
	int time; // ��¼�����ڴ�ʱ��
};

Page b[bsize];			  // �ڴ浥Ԫ��
int result[bsize][psize]; // ������н��
int phb[bsize] = {0};	  // �������
int pro[psize] = {0};	  // �������к�
int missPage = 0;		  // ͳ��ҳ��ȱҳ����

void buildNum()
{
	// ����������кź���
	printf("�������һ���������к�Ϊ��\n");
	int i = 0;
	for (i = 0; i < psize; i++)
	{
		pro[i] = 10 * rand() / (RAND_MAX + 1) + 1;
		std::cout << pro[i] << " ";
	}
	std::cout << std::endl;
	return;
}

// ��ʼ���ڴ浥Ԫ��������
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
	printf("\nȱҳ����Ϊ��%6d\nȱҳ�ʣ�%16.6f\n", missPage, (float)(missPage) / psize);
}

void FIFO()
{
	buildNum();
	Init(b, result);
	int avbBlock = -1, samProc = -1;
	int maxWait = -1, maxProc = 0; // ����滻���������±�
	int waitTimes[bsize] = {0};	   // FIFO ���� ���̵ȴ�����(������δ��ʹ�õĽ��̱�־)
	// �Ƚ��ȳ�ҳ���û��㷨
	for (int i = 0; i < psize; i++)
	{
		avbBlock = samProc = -1;
		maxWait = -1, maxProc = 0;
		for (int j = 0; j < bsize; j++)
		{
			// ���ҿ��������
			if (phb[j] == 0 && avbBlock == -1)
				avbBlock = j;
			// ������ͬ����
			if (phb[j] == pro[i] && samProc == -1)
				samProc = j;
			// ��waitTimesֵ����
			if (waitTimes[j] > maxProc && maxWait == -1)
			{
				maxProc = waitTimes[j];
				maxWait = j;
			}
		}

		// ��������ͬ���̡����ڿ��������
		if (samProc == -1 && avbBlock != -1)
		{
			phb[avbBlock] = pro[i]; // ���̺�����ÿ��������
			missPage++;				// ȱҳ
			waitTimes[avbBlock] = 0;
			for (int j = 0; j <= avbBlock; j++)
				waitTimes[j]++;
		}
		// ��������ͬ���̡������ڿ��������
		else if (samProc == -1 && avbBlock == -1)
		{
			phb[maxWait] = pro[i];
			waitTimes[maxWait] = 0;
			for (int j = 0; j < bsize; j++)
				waitTimes[j]++;
			missPage++; // ȱҳ
		}
		// ������ͬ�Ľ���
		else if (samProc != -1)
		{
			phb[samProc] = pro[i];
			for (int j = 0; j < bsize; j++)
				waitTimes[j]++;
		}
		// ��¼���
		for (int j = 0; j < bsize; j++)
			result[j][i] = phb[j];
	}

	Output();
}

void _LRU(int fold, Page *b)
{
	// LRU���Ĳ���
	int samProc = -1;
	// �ж�ҳ���Ƿ������ڴ���
	for (int i = 0; i < bsize; i++)
	{
		if (fold == b[i].num)
			samProc = i;
	}

	// ������ͬ����
	if (samProc >= 0)
	{
		b[samProc].time = 0;
		for (int i = 0; i < bsize; i++)
			if (i != samProc)
				b[i].time++;
	}
	// ��������ͬ����
	else
	{
		// ȡ�����ڴ���ͣ����õ�ҳ��,Ĭ��״̬��Ϊ��������ҳ��
		int Longest = -1;
		for (int i = 0; i < bsize; i++)
		{
			if (b[i].time > Longest)
			{
				Longest = b[i].time;
				samProc = i;
			}
		}
		++missPage; // ȱҳ
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

		// ��¼���
		for (int j = 0; j < bsize; j++)
			result[j][i] = b[j].num;
	}
	// ������
	Output();
}

int main()
{
	int choice;
	while (1)
	{
		std::cout << "--------------------" << std::endl;
		std::cout << " 0 �� Exit	" << std::endl;
		std::cout << " 1 �� FIFO	" << std::endl;
		std::cout << " 2 �� LRU	" << std::endl;
		std::cout << "--------------------" << std::endl;
		std::cout << "ѡ��";
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