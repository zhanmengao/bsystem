#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//�����������ĵ�ַ ���ᱨ��
char * getStr1()
{
	char *p1 = "abcdefg2";
	return p1;
}
char *getStr2()
{
	char *p2 = "abcdefg2";        
	return p2;
}

#ifdef __linux
int main()
#else
int main501a()
#endif
{
	char *p1 = NULL;
	char *p2 = NULL;
	p1 = getStr1();
	p2 = getStr2();

	//��ӡp1 p2 ��ָ���ڴ�ռ������(str)
	printf("p1:%s , p2:%s \n", p1, p2);

	//��ӡp1 p2 ��ֵ
	printf("p1:%d , p2:%d \n", p1, p2);

	//��ӡp1 p2 �ĵ�ַ
	printf("&p1:%d , &p2:%d \n", &p1, &p2);
    return 0;
}