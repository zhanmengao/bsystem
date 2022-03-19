#pragma once
#include<string.h>
template<typename T> //ģ�庯��
int compare(const T &v1, const T &v2)
{
	if (v1 > v2) return -1;
	if (v2 > v1) return 1;
	return 0;
}
//ģ��������,��������ַ����ض��ıȽϣ�Ҫ�ṩ����ʵ�Σ�����ֻ��һ��T
template<>
int compare(const char* const &v1, const char* const &v2)
{
	return strcmp(v1, v2);
}