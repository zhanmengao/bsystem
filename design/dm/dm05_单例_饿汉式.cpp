
#include <iostream>
using namespace std;


//懒汉式
class Singelton
{
private:
	Singelton()
	{
		cout << "Singelton 构造函数执行" << endl;
	}
public:
	static Singelton *getInstance()
	{
		return m_psl;
	}

	static void FreeInstance()
	{
		if (m_psl != NULL)
		{
			delete m_psl;
			m_psl = NULL; 
		}
	}

private:
	static Singelton *m_psl;
};

//int g_count = 0;
//饿汉式
Singelton *Singelton::m_psl = new Singelton;


void main041()
{
	printf("sss\n");
	Singelton *p1 = Singelton::getInstance();
	Singelton *p2 = Singelton::getInstance();

	if (p1 == p2)
	{
		cout << "是同一个对象" << endl;
	}
	else
	{
		cout << "不是同一个对象" << endl;
	}
	Singelton::FreeInstance();

	return ;
}

void main()
{
	main041();

	system("pause");
}