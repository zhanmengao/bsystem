#include<iostream>
#include <string>
using namespace std;
//������
int MaxChengji(int *array, int size)
{
	int max = 0;
	//�ҵ���ͷ
	for (int i = 0; i < size; i++)
	{
		//�ҵ�ÿһ����β
		for (int j = i; j < size; j++)
		{
			int chengji = 1;
			//�۳ˣ���i�˵�j
			for (int k = i; k <= j; k++)
			{
				chengji *= array[k];
				if (chengji > max)
				{
					max = chengji;
				}
			}
		}
	}
	return max;
}
//�������Ż�
int BetterMaxChengji(int *array, int size)
{
	//�ҵ���ͷ
	for (int i = 0; i < size; i++)
	{
		//�ҵ���β
		for (int j = i; j < size; j++)
		{
			int chengji = 1;

			
		}
	}
	return 0;
}
void main444()
{
	int array[] = { 2,5,-1,2,-1 };
	cout<<MaxChengji(array,5)<<endl;
}