
/*

#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void main()
{
	printf("hello...\n");
	system("pause");
	return ;
}
*/

#include "iostream" //����c++��ͷ�ļ� //iostream.h

using namespace std; //ʹ�������ռ� std ��׼�������ռ� (����������ռ��ж����˺ܶ��׼����)

void main101()
{
	//printf("hello....\n");

	//cout ��׼��� ����Ļ, 
	//<< ���Ʋ�����  ��c++���� ���ܵĸ���(��ǿ)===>C++ ���Բ���������  //3day
	//endl \n
	//��ӡ����Ļ ���һ���
	cout << "hello...."<<endl;

	system("pause");
}

//��Բ�����
//��������̵ķ������Բ�ε����
// << >> ������: ��׼����cin�ͱ�׼���cout ʼ��д�ڲ���������� 
void main102()
{
	double	r = 0;
	double	s = 0;

	cout<<"������Բ�εİ뾶:";

	//cin ��׼���� �������
	cin >> r;
	cout<<"r��ֵ��:" << r <<endl;
	s = 3.14*r*r;
	cout<<"Բ�ε������s:" <<s << endl;
	system("pause");
}

//��C++�� ������һ���Զ�����������
struct Circle01
{
	double	m_s; //Բ�����
	double	m_r; //Բ�İ뾶
};

//��C++�� ������һ���Զ����������� MyCicle Բ��
class MyCicle
{
public:  
	double	m_s; //Բ����� ���� ��Ա����
	double	m_r; //Բ�İ뾶

public:
	void setR(double r) //��Ա����
	{
		m_r = r;
	}

	double getR() //��Ա����
	{
		return m_r;
	}
	double getS()
	{
		m_s = 3.14*m_r*m_r;
		return m_s;
	}
};

//

void main103()
{
	MyCicle		c1, c2, c3; //���� ���� ���� ����
	double		r;
	cout << "������c1Բ�εİ뾶:";
	cin >> r;

	//��c1Բ�ε����Ը�ֵ
	c1.setR(r);

	cout << "c1Բ�ε������:" << c1.getS()<<endl;

	system("pause");
}

//���������ķ���
//1 ��ĳ��� ��Ա�����ͳ�Ա���� 
//2 ʵ���� ��Ķ���
//3 �����
	//������̼ӹ����� һ��һ���ĺ��� 
	//�������ӹ�����:һ��һ������
//4 main���ɲ���

//˼��1:��ĵ��� ִ�й��̷���==>����벻��һ��һ��ָ��
//����һ����������,(�̶���С�ڴ��ı���); ����һ����,��һ������ĸ���,�����������ڴ�
//���������Ͷ��������ʱ��,�Ż�����ڴ�
//˼��2:��ש: c++����������δ��� �������,������ĳ�Ա������;����һ��:���������c1 c2 c3 ������getS,��ש.....

void main104()
{
	MyCicle		c1, c2, c3; //���� ���� ���� ����
	double		r1, r2, r3;
	
	//Բ��1
	cout << "������c1Բ�εİ뾶r1:";
	cin >> r1;
	//��c1Բ�ε����Ը�ֵ
	c1.setR(r1);
	cout << "c1Բ�ε������:" << c1.getS()<<endl;


	//Բ��2
	cout << "������c2Բ�εİ뾶r2:";
	cin >> r2;
	//��c1Բ�ε����Ը�ֵ
	c2.setR(r2);
	cout << "c2Բ�ε������:" << c2.getS()<<endl;

	system("pause");
}






