

#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//������ڴ�
char *getMem161(int num)
{
	char *p1 = NULL;
	p1 = (char *)malloc(sizeof(char) * num);
	if (p1 == NULL)
	{
		return NULL;
	}
	return p1;
}

//ջ
//ע�� return���ǰ��ڴ�� 64���ֽ�,��return����,���ǰ��ڴ����׵�ַ(�ڴ�ı��0xaa11) ,���ظ� tmp

// ���ָ��Ĺؼ�,���ڴ�. û���ڴ������ָ�� 

//�������Ϊ������ջ�ڴ棬���ڴ��ں����˳����Ѿ�������
char *getMem162()
{
	char buf[64]; //��ʱ���� ջ�����
	strcpy(buf, "123456789");
	return buf;
}

void main601()
{
	char *tmp = NULL;
	tmp = getMem161(10);
	if (tmp == NULL)
	{
		return ;
	}
	strcpy(tmp, "111222"); //��tmp��ָ����ڴ�ռ���copy����
	printf("getMem= %s \n", tmp);

	tmp = getMem162();
	printf("getMem2= %s \n", tmp);
	tmp = 0xaa11;

}