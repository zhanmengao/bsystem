
#define  _CRT_SECURE_NO_WARNINGS 
#include <iostream>
using namespace std;

//

class  Name
{
public:
	Name(const char *myp)
	{
		m_len = strlen(myp);
		m_p =(char *) malloc(m_len + 1); //
		strcpy(m_p, myp);
	}

	//Name obj2 = obj1;
	//解决方案: 手工的编写拷贝构造函数 使用深copy
	Name(const Name& obj1)
	{
		m_len = obj1.m_len;
		m_p = (char *)malloc(m_len + 1);
		strcpy(m_p, obj1.m_p);
	}

	//obj3 = obj1;  // C++编译器提供的 等号操作 也属 浅拷贝
	//obj3.operator=(obj1)

	Name& operator=(Name &obj1)
	{
		//先释放旧的内存
		if (this->m_p != NULL)
		{
			delete[] m_p;
			m_len = 0;
		}
		//2 根据obj1分配内存大小
		this->m_len = obj1.m_len;
		this->m_p = new char [m_len+1];
		
		//把obj1赋值
		strcpy(m_p, obj1.m_p);
		return *this;
	}
	
	~Name()
	{
		if (m_p != NULL)
		{
			free(m_p);
			m_p = NULL;
			m_len = 0;
		}
	}
protected:
private:
	char *m_p ;
	int m_len; 
};

//对象析构的时候 出现coredump
void objplaymain()
{
	Name obj1("abcdefg");
	Name obj2 = obj1;  //C++编译器提供的 默认的copy构造函数  浅拷贝
	Name obj3("obj3");

	obj3 = obj1;  // C++编译器提供的 等号操作 也属 浅拷贝
	//obj3.operator=(obj1)
	//operato=(Name &obj1)

	obj1 = obj2 = obj3;
	//obj2.operator=(obj3);
	//obj1 = void;
}

void main401()
{
	objplaymain();
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}