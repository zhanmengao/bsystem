/*
1、编写一个函数模板，它返回两个值中的较小者，同时要求能正确处理字符串。
思路：
这里设计一个函数模板template<class T> T min(T a,T b)，
可以处理int、float和char 等数据类型，为了能正确处理字
符串，添加一个重载函数专门处理字符串比较，
即char *min(char *a,char *b)。
*/
#include<iostream> 
#include<string.h> 
using namespace std;

template<class T>
T min(T a, T b)
{
	return (a < b ? a : b);
}

char *min(char *a, char *b)
{
	return (strcmp(a, b) < 0 ? a : b);
}

int main(int argc, char **argv)
{
	double a = 3.56, b = 8.23;
	char s1[] = "Hello", s2[] = "Good";
	cout << "输出结果:" << endl;
	cout << " " << a << "," << b << "中较小者:" << min(a, b) << endl;
	cout << " " << s1 << "," << s2 << "中较小者:" << min(s1, s2) << endl;
	system("pause");
	return 0;
}