
#include <iostream>
using namespace std;

//public ���εĳ�Ա���� ���� ������ڲ� ����ⲿ����ʹ��
//protected: ���εĳ�Ա��������,������ڲ�ʹ�� ,�ڼ̳е������п��� ;���� ����ⲿ���ܱ�ʹ��
//private: ���εĳ�Ա�������� ֻ��������ڲ�ʹ�� ����������ⲿ

//��������ʿ��ƵĽ���
//1 protected �ؼ��� ���εĳ�Ա���� �ͳ�Ա���� ,��Ϊ���ڼ�����ʹ�� ,��Ϊ�˼̳�
//2 ��Ŀ������ һ������� �� public
//3


class Parent201
{
public:
			int a; //�ϵ�������
protected:
			int b; //�ϵ�����������
private:
			int c; //�ϵ�������
};


//�����̳�
class Child203 : protected Parent201
{
public:
protected:
private:
public:	
	void useVar()
	{
		a = 0; // ok
		b = 0; // ok //b������ д��������Child3���ڲ�  2��protected 3����===>protected
		//c = 0; //err
	}
};

void main201()
{
	Child203 c3;
	//c3.a = 10; //err
	//c3.b = 20; //err
	//c3.c = 30;//err

}



//˽�м̳�
class Child202 : private Parent201
{
public:
	void useVar()
	{
		a = 0; //ok
		b = 0; //ok
		//c = 0; // err
	}

protected:
private:
};

void main202()
{
	Child202 c2;
	//c1.a = 10; err
	//c2.b = 20; err
	//c3.b = 30;
}


//public�̳�
class Child201 : public Parent201
{
public:
	void useVar()
	{
		a = 0; //ok
		b = 0; //ok
		//c = 0; // err
	}

protected:
private:
};


/*
C++�еļ̳з�ʽ��public��private��protected����Ӱ������Ķ����������	
	�ж�ĳһ�仰���ܷ񱻷���
	1����������䣬��仰д��������ڲ����ⲿ
	2����������δӸ���̳У�public��private��protected��	
	3���������еķ��ʼ���public��private��protected��
*/
//���м̳� 
void main203()
{
	Parent201 t1, t2;
	t1.a  = 10; //ok
	//t1.b = 20; //err
	//t2.c = 30 ; //err
	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

