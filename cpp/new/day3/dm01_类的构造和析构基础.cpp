
#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;

class Test1
{
public:
	Test1()  //�޲��� ���캯��
	{
		a = 10;  //������ɶ����Եĳ�ʼ������
		p = (char *)malloc(100);
		strcpy(p, "aaaaffff");
		cout<<"���ǹ��캯�� ��ִ����"<<endl;
	}
	void print()
	{
		cout<<p<<endl;
		cout<<a<<endl;
	}
	~Test1() //��������
	{
		if (p != NULL)
		{
			free(p);
			p = nullptr;
		}
		cout<<"������������,��������" <<endl;
	}
protected:
private:
	int a ;
	char *p;
};

//������һ����̨,�о��������Ϊ
void objplay()
{
	//�ȴ����Ķ��� ���ͷ�
	Test1 t1;
	t1.print();

	printf("�ָ���\n");
	Test1 t2;
	t2.print();
}
void main101()
{
	objplay();
	system("pause");
	return ;
}