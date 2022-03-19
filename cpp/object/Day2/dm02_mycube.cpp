

// ����1	�����������(cube)��������������������
//			�����������壬�Ƿ���ȣ�ȫ�ֺ����ͳ�Ա������

#include <iostream>
using namespace std;

class Cube
{
public:
	void setA(int a)
	{
		m_a = a;
	}
	void setB(int b)
	{
		m_b = b;
	}
	void setC(int c)
	{
		m_c = c;
	}

	void setABC(int a=0, int b=0, int c=0)
	{
		m_a = a;  m_b = b; m_c = c;
	}

	int getA()
	{
		return m_a;
	}
	int getB()
	{
		return m_b;
	}
	int getC()
	{
		return m_c;
	}

public:
	int getV()
	{
		m_v = m_a*m_b*m_c;
		return m_v;
	}

	int getS()
	{
		m_s = 2*(m_a*m_b + m_a*m_c + m_b*m_c);
		return m_s;
	}
	
	int judgeCube(Cube &v1, Cube &v2)  //3k
	{
		if ( (v1.getA() == v2.getA()) &&
			(v1.getB() == v2.getB()) &&
			(v1.getC() == v2.getC())      )
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	int judgeCube(Cube &v2)  //��Ա���� �������� //4k
	{
		//�ö�ѧԱ �ֲ��� ��������µ�m_a ������v1 ���� ����v2 vn
		if ( (m_a == v2.getA()) &&
			(m_b == v2.getB()) &&
			(m_c == v2.getC())     ) 
		{
			return 1;
		}
		else
		{
			return 0;
		}

		return 0;
	}

private:
	int m_a;
	int m_b;
	int m_c;
	int m_v;
	int m_s;
};


//ȫ�ֺ��� PK ��Ա����
//1��� 0�����
int judgeCube(Cube &v1, Cube &v2)
{
	if (	(v1.getA() == v2.getA()) &&
			(v1.getB() == v2.getB()) &&
			(v1.getC() == v2.getC())
		)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void main201()
{
	Cube v1, v2;
	v1.setABC(1, 2, 3);
	cout<<v1.getS()<<endl;
	cout<<v1.getV()<<endl;
	cout<<"hello..."<<endl;

	//
	v2.setABC(1,2,4);

	//�����׳� ����
	//�������� ��ζ��: ȥִ�� v1�������ĳ�ԱjudgeCube ,
	int tag = v1.judgeCube(v2);  //�ж�v1������ �� v2�������Ƿ���� ,ͨ����ĳ�Ա����
	if ( tag == 1)
	{
		cout<<"���"<<endl;
	}
	else
	{
		cout<<"�����"<<endl;
	}

	
	//�ж������������Ƿ����
	if ( v1.judgeCube(v1, v2) == 1)
	{
		cout<<"���"<<endl;
	}
	else
	{
		cout<<"�����"<<endl;
	}

	system("pause");
	return ;
}
//ȫ�ֺ�������
void main202()
{
	Cube v1, v2;
	v1.setABC(1, 2, 3);
	cout<<v1.getS()<<endl;
	cout<<v1.getV()<<endl;
	cout<<"hello..."<<endl;

	//
	v2.setABC(1,2,4);

	//�ж������������Ƿ����
	if (judgeCube(v1, v2) == 1)
	{
		cout<<"���"<<endl;
	}
	else
	{
		cout<<"�����"<<endl;
	}
	

	system("pause");
	return ;
}