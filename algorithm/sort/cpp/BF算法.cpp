#include<iostream>
#include <string>
using namespace std;

//�����ڵڼ�λ����
int BF(const string &zhu, const string &zi,int pos=0)
{
	//������ǰλ��
	int i = pos;
	//�Ӵ���ǰλ��
	int j = 0;
	//iС����������ʱ�˳�ѭ��������-1
	while (i < zhu.length())
	{
		if (zhu[i] == zi[j])
		{
			//����
			i++;
			j++;
			//�����ʱƥ������
			if (j == zi.length())
			{
				return i-j+1;
			}
		}
		else
		{
			//�˻ر��ο�ʼƥ��ĵط���һλ
			i = i - j + 1;
			j = 0;
		}
	}
	return -1;
}
