#pragma once
#include<stdlib.h>
#include<assert.h>

void mainmalloc()
{
	char *str = (char*)malloc(100);
	str = (char*)realloc(str, 50);				//������ǰ������ڴ泤�ȣ����ӻ���٣���
	assert(str != nullptr);
	free(str);
	str = nullptr;

	//Ϊָ�����ȵĶ��󣬷�����������ָ���������ڴ档���뵽���ڴ��ÿһλ��bit������ʼ��Ϊ 0��
	str = (char*)calloc(100, sizeof(char));		
	assert(str != nullptr);
	free(str);
	str = nullptr;

	int* t = new int();     // ���ڴ���� ���ٹ��캯��
	delete t;           // ���������������ڴ��ͷ�


}