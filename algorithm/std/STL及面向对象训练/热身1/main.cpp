/*
1����дһ������ģ�壬����������ֵ�еĽ�С�ߣ�ͬʱҪ������ȷ�����ַ�����
˼·��
�������һ������ģ��template<class T> T min(T a,T b)��
���Դ���int��float��char ���������ͣ�Ϊ������ȷ������
���������һ�����غ���ר�Ŵ����ַ����Ƚϣ�
��char *min(char *a,char *b)��
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
	cout << "������:" << endl;
	cout << " " << a << "," << b << "�н�С��:" << min(a, b) << endl;
	cout << " " << s1 << "," << s2 << "�н�С��:" << min(s1, s2) << endl;
	system("pause");
	return 0;
}