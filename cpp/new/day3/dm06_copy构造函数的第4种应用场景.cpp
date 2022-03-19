


#include <iostream>
using namespace std;


class Location6
{ 
public:
	Location6( int xx = 0 , int yy = 0 ) 
	{ 
		X = xx ;  Y = yy ;  cout << "Constructor Object.\n" ; 
	}

	//copy���캯��  ��ɶ���ĳ�ʼ��
	Location6(const Location6 & obj) //copy���캯�� 
	{
		X = obj.X; Y = obj.Y;
		cout << "Copy Create Object.\n";
	}
	Location6& operator=(const Location6& obj)
	{
		X = obj.X; Y = obj.Y;
		cout << "operator= Object.\n";
		return *this;
	}
	~Location6() 
	{ 
		cout << X << "," << Y << " Object destroyed." << endl ; 
	}
	int  GetX () { return X ; }		
	int GetY () { return Y ; }
private :   
	int  X , Y ;
} ;

//g���� ����һ��Ԫ�� 
//����1 : �����ķ���ֵ��һ��Ԫ�� (�������͵�), ���ص���һ���µ���������(���Ի���������������copy���캯��)

//
//����2: �й� ���������ȥ����
//�������������  ��ʼ�� ����һ��ͬ���͵Ķ���, �������� ת����������
//�������������  ��ֵ�� ����һ��ͬ���͵Ķ���, �������� ������

//
//����ôд����,��Ʊ������Ĵ�ţ��:
//�Ҿ͸��㷵��һ���¶���(û������ ��������)
Location6 g()
{
	cout << "G Begin-----------------------------------" << endl;
	Location6 A(1, 2);
	cout << "G End-----------------------------------" << endl;
	return A;
}

//
void objplay2()
{
	g(); 
}

//
void objplay3()
{
	//�����������ʼ��m ��ʱc++������ ֱ�Ӱ�������ת��m;(����) ������ת����������m
	Location6 m = g(); 
	printf("��������,������,����������\n");
	cout<<m.GetX()<<endl;;
}

void objplay4()
{
	//���������� ��ֵ�� m2��, ������������
	Location6 m2(1, 2);
	m2 = g();
	printf("��Ϊ����������=��m2, ��������,������\n");
	//cout<<m2.GetX()<<endl;;
}
void main333()
{
	//objplay2();
	//objplay3();
	objplay4();
	cin.get();
}