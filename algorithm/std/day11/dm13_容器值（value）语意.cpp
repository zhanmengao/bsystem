#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
using namespace std;
#include "vector"

class Teacher
{
public:
	Teacher(char *name, int age)
	{
		m_pname = new char[strlen(name) + 1];
		strcpy(m_pname, name);
		m_age = age;
	}
	~Teacher()
	{
		if (m_pname != NULL)
		{
			delete[] m_pname;
			m_pname = NULL;
			m_age = 0;
		}
	}

	//Teacher t2 = t1;
	Teacher(const Teacher &obj)
	{
		cout << "Teacher拷贝构造函数被调用" << endl;
		m_pname = new char[strlen(obj.m_pname)+1];
		strcpy(m_pname, obj.m_pname);
		m_age = obj.m_age;
	}

	//重载 =号操作符
	//t3 = t2 = t1
	Teacher& operator=(const Teacher &obj)
	{
		//先把t2的旧的内存释放掉
		if (m_pname != NULL)
		{
			delete[] m_pname;
			m_pname = NULL;
			m_age = 0;
		}

		//根据t1的大小分配内存 
		m_pname = new char[strlen(obj.m_pname) +1 ];
		//copy t1的数据
		strcpy(m_pname, obj.m_pname);
		m_age = obj.m_age;
		return *this;
	}

public:

	void printT()
	{
		cout << m_pname << "\t" << m_age << endl;
	}
protected:
private:
	char	*m_pname;
	int		m_age;
};

void main1301()
{
	Teacher t1("t1", 31);
	t1.printT();

	vector<Teacher> v1;
	v1.push_back(t1); //把t1拷贝了一份 存入到容器中了....
}