
#include <iostream>
using namespace std;

//inline void printA();

//inline int myfunc(int a, int b)�ͺ������ʵ�֣�д��һ��

inline void printA()
{
	int a = 10;
	cout<<"a"<<a<<endl;
}

void main1401()
{
	printA();
	//2 C++������ֱ�ӽ�����������ں������õĵط�
	//3 ��������ʡȥ����ͨ��������ʱѹջ����ת�ͷ��صĿ��� 
// 	{
// 		int a = 10;
// 		cout<<"a"<<a<<endl;
// 	}

	cout<<"hello..."<<endl;
	system("pause");
	return ;
}


//�������ĺ�
#define MYFUNC(a, b) ((a) < (b) ? (a) : (b))  

inline int myfunc(int a, int b)
{
	return a < b ? a : b;
}

int main1402()
{
	int a = 1;
	int b = 3;
	//int c = myfunc(++a, b);  // a=2 b=3 c=2
	int c = MYFUNC(++a, b); //==>���滻��չ�� ((++a) < (b) ? (++a) : (b))  //a=3 b=3 c=3


	printf("a = %d\n", a); 
	printf("b = %d\n", b);
	printf("c = %d\n", c);

	system("pause");
	return 0;
}
