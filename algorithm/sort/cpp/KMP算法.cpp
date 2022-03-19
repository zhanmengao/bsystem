#include<iostream>
#include <string>
using namespace std;
void getNext(const string &zi, int* next);
static int KMP(const string &fu, const string &zi, int pos = 0)
{
	int i = pos;
	int j = 0;
	//��ȡNext����
	int nextArr[255] = { 0 };
	getNext(zi, nextArr);
	//��ʼƥ��
	while (i < fu.length())
	{
		//�Ƚ�
		if (j == 0 || fu[i] == zi[j])
		{
			//����
			i++;
			j++;
			//�����ʱƥ������
			if (j == zi.length())
			{
				return i - j + 1;
			}
		}
		//����ʱʧ��->�˻�ǰһ������ƥ���м������
		else if (j > 0)
		{
			j = nextArr[j];
		}
	}
	return -1;
}
//�õ��Ӵ���next���顪����jλ��ʧ�䣬��ǰ���м����������
//ǰ����0�������Ϊ0����1�������Ϊ1...
void getNext(const string &zi,int* next)
{
	//ǰ׺����
	int b = 0;
	//��׺����
	int cur = 1;
	while (cur < zi.length())
	{
		//��ӡһ������
		for (int i = 0; i < zi.length(); i++)
		{
			printf("Before	next [%u] : %d \n", i, next[i]);
		}
		printf("\n");
		//������
		if (b == 0 ||  zi[b] == zi[cur])
		{
			b++;
			cur++;
			next[cur] = b;
		}
		//����ȵ����
		else
		{
			//��˷����ǰ�ַ���ǰ׺���׺������������ڴ˴�����P��T�Ĳ�ƥ��
			b = next[b];
		}
		//��ӡһ������
		for (int i = 0; i < zi.length(); i++)
		{
			printf("After	next [%u] : %d \n", i, next[i]);
		}
		printf("\n");
	}
}
void mainKMP()
{
	string fu = "abcababcaabcabx";
	string zi = "abcabx";
	cout << KMP(fu, zi) << endl;
}