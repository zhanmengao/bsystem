

// 案例1	设计立方体类(cube)，求出立方体的面积和体积
//			求两个立方体，是否相等（全局函数和成员函数）

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

	int judgeCube(Cube &v2)  //成员函数 函数重载 //4k
	{
		//好多学员 分不清 这个场景下的m_a 是属于v1 还是 属于v2 vn
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


//全局函数 PK 成员函数
//1相等 0不相等
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

	//问题抛出 冗余
	//函数调用 意味着: 去执行 v1对象的类的成员judgeCube ,
	int tag = v1.judgeCube(v2);  //判断v1立方体 和 v2立方体是否相等 ,通过类的成员函数
	if ( tag == 1)
	{
		cout<<"相等"<<endl;
	}
	else
	{
		cout<<"不相等"<<endl;
	}

	
	//判断两个立方体是否相等
	if ( v1.judgeCube(v1, v2) == 1)
	{
		cout<<"相等"<<endl;
	}
	else
	{
		cout<<"不相等"<<endl;
	}

	system("pause");
	return ;
}
//全局函数调用
void main202()
{
	Cube v1, v2;
	v1.setABC(1, 2, 3);
	cout<<v1.getS()<<endl;
	cout<<v1.getV()<<endl;
	cout<<"hello..."<<endl;

	//
	v2.setABC(1,2,4);

	//判断两个立方体是否相等
	if (judgeCube(v1, v2) == 1)
	{
		cout<<"相等"<<endl;
	}
	else
	{
		cout<<"不相等"<<endl;
	}
	

	system("pause");
	return ;
}