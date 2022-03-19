
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
		m_p =(char *) malloc(m_len + 1); 
		strcpy(m_p, myp);
	}

	//Name obj2 = obj1;
	//�������: �ֹ��ı�д�������캯�� ʹ����copy
	Name(const Name& obj1)
	{
		m_len = obj1.m_len;
		m_p = (char *)malloc(m_len + 1);
		strcpy(m_p, obj1.m_p);
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
public:
	char *m_p;
	int m_len;
};

//����������ʱ�� ����coredump
void objplaymain()
{
	Name obj1("abcdefg");
	Name obj2 = obj1;  //C++�������ṩ�� Ĭ�ϵ�copy���캯��  ǳ����
	Name obj3("obj3");

	char *p = obj3.m_p;//����ԭ�����ڴ�
	obj3 = obj1;  // C++�������ṩ�� �ȺŲ��� Ҳ�� ǳ����
	strcpy(obj1.m_p, "����obj1������");
	printf("obj3.p = %s \n", obj3.m_p);
}

int main901()
{
	objplaymain();
	return 0;
}