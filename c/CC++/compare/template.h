#pragma once
#include<string.h>
template<typename T> //模板函数
int compare(const T &v1, const T &v2)
{
	if (v1 > v2) return -1;
	if (v2 > v1) return 1;
	return 0;
}
//模板特例化,满足针对字符串特定的比较，要提供所有实参，这里只有一个T
template<>
int compare(const char* const &v1, const char* const &v2)
{
	return strcmp(v1, v2);
}