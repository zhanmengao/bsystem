
#include <iostream>
using namespace std;

//C�����еı�����������������ʼ��λ�ö��壡��
//C++�и�ǿ�����Եġ�ʵ���ԡ������еı�������������Ҫʹ��ʱ�ٶ��塣

void main401()
{
	int i;

	printf("hello...\n");

	int k;

	i = 10;
	k = 11;
	printf("i:%d k:%d \n", i, k);
	system("pause");

}

void main402()
{
	register int a = 0; 
	printf("&a: %d \n", &a); //�����ڼĴ���������ȡ��ַ

	for (int i=0; i<1000; i++) //��ʹ��registerҲ�������Ż�
	{
		printf("i:%d \n", i);
	}
	system("pause");
}



/*
��C�����У��ظ�������ͬ����ȫ�ֱ����ǺϷ���
	��C++�У�����������ͬ����ȫ�ֱ���
C�����ж��ͬ����ȫ�ֱ������ջᱻ���ӵ�ȫ����������ͬһ����ַ�ռ���
int g_var;
int g_var = 1;

C++ֱ�Ӿܾ����ֶ����Ե�������
*/

int g_a = 100;
//int g_a ;

void main403()
{
	printf("hello...g_a:%d \n", g_a);
}



