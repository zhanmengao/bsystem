
#include <iostream>
using namespace std;

//��д ���� �ض���
//��д������2����֮��
//���ر�����һ����֮��

//��д��Ϊ2��
//1 �麯����д  ��������̬
//2 ���麯����д ���ض��壩

class Parent
{
	//������������������ع�ϵ
public: 
	void abc()
	{
		printf("abc");
	}

	virtual void func() 
	{
		cout<<"func() do..."<<endl;
	}
	virtual void func(int i)
	{
		cout<<"func() do..."<<i<<endl;
	}
	virtual void func(int i, int j)
	{
		cout<<"func() do..."<<i<< " "<<j<<endl;
	}

	virtual void func(int i, int j, int m , int n)
	{
		cout<<"func() do..."<<i<< " "<<j<<endl;
	}
protected:
private:
};


class Child : public Parent
{
	
public: 
	void abc()
	{
		printf("child abc");
	}
	/*
	void abc(int a)
	{
		printf("child abc");
	}
	*/
	virtual void func(int i, int j)
	{
		cout<<"func(int i, int j) do..."<<i<< " "<<j<<endl;
	}
	virtual void func(int i, int j, int k)
	{
		cout<<"func(int i, int j) do.."<< endl; 
	}
protected:
private:
};


//������д���ض���
void main1601()
{
	//: error C2661: ��Child::func��: û�����غ������� 0 ������
	Child c1;

	//c1.func();
	//�����޷����ظ���ĺ���������ͬ�������������Ƹ���  ->>�����ض���
	c1.Parent::func();

	//1 C++������ ����func���� ,��������func�����Ѿ�������(���Ƹ���).����c++����������ȥ�Ҹ����4��������func����
	//2 c++������ֻ����������,����func����,�ҵ�������func,һ����2��������,һ����3��������.
	//3 C++��������ʼ����.....  error C2661: ��Child::func��: û�����غ������� 4 ������
	//4 ������ø����func,ֻ�ܼ��ϸ��������..����ȥ����..
	//c1.func(1, 3, 4, 5);

	//c1.func();
	//func���������֣��������з��������Ƹ��ǣ�����ĺ��������֣�ռ���˸���ĺ��������ֵ�λ��
	//��Ϊ�������Ѿ�����func���ֵ�������ʽ��������
	//��������ʼ����������func����������������û��0��������func���� 


	cout<<"hello..."<<endl;
	system("pause");
	return ;
}
