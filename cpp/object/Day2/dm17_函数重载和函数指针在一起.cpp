
#include <iostream>
using namespace std;

void myfunc(int a)
{
	printf("a:%d \n", a);
}

void myfunc(char *p)
{
	printf("%s \n", p);
}


void myfunc(int a, int b)
{
	printf("a:%d \n", a);
}

void myfunc(char *p1, char *p2)
{
	printf("p1:%s ", p1);
	printf("p2:%s \n", p2);
}

//����ָ�� �������﷨
//1����һ����������
typedef void (myTypeFunc)(int a,int b) ;  //int
//myTypeFunc *myfuncp = NULL; //����һ������ָ�� ���ָ��ָ��������ڵ�ַ

//����һ������ָ������ 
typedef void (*myPTypeFunc)(int a,int b) ;  //������һ��ָ����������� 
//myPTypeFunc fp = NULL;  //ͨ��  ����ָ������ ������ һ������ָ�� ,

//����һ������ָ�� ����
void (*myVarPFunc)(int a, int b);
//

void main1701()
{
	myPTypeFunc fp; //������һ�� ����ָ�� ����  

	fp = myfunc;
	//fp(1);
	
	//myVarPFunc = myfunc;

	fp(1, 2);

	/*
	{
		char buf1[] = "aaaaafff";
		char buf2[] = "bbbb";
		fp(buf1, buf2);
	}
	*/
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}