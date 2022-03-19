
#include <iostream>
using namespace std;
#include "string"

//Caretaker ������
// MememTo  ����¼

class MememTo
{
public:
	MememTo(string name,int age )
	{
		m_name = name;
		m_age = age;
	}
	string getName()
	{
		return m_name;
	}
	int getAge()
	{
		return m_age;
	}

	void setName(string name)
	{
		this->m_name = name;
	}
	void setAge(int age)
	{
		this->m_age = age;
	}
private:
	string	m_name;
	int		m_age;
};

class  Person
{
public:
	Person(string name,int age )
	{
		m_name = name;
		m_age = age;
	}
	string getName()
	{
		return m_name;
	}
	int getAge()
	{
		return m_age;
	}

	void setName(string name)
	{
		this->m_name = name;
	}
	void setAge(int age)
	{
		this->m_age = age;
	}

	//����
	MememTo* createMememTo()
	{
		return new MememTo(m_name, m_age);
	}
	//��ԭ 
	void setMememTo(MememTo* memto)
	{
		this->m_age = memto->getAge();
		this->m_name = memto->getName();
	}
public:
	void printT()
	{
		cout << "m_name:" << m_name << " m_age:" << m_age << endl;
	}
private:
	string	m_name;
	int		m_age;
};

class Caretaker
{
public:
	Caretaker(MememTo *memto)
	{
		this->memto = memto;
	}
	MememTo *getMememTo()
	{
		return memto;
	}
	void setMememTo(MememTo *memto)
	{
		this->memto = memto;
	}
protected:
private:
	MememTo *memto;
};

void main26_02()
{
	//MememTo *memto = NULL;
	Caretaker *caretaker = NULL;
	Person *p = new Person("����", 32);
	p->printT();

	//���� Person�����һ��״̬
	printf("---------\n");
	caretaker = new Caretaker( p->createMememTo());
	p->setAge(42);
	p->printT();

	printf("��ԭ�ɵ�״̬\n" );
	p->setMememTo(caretaker->getMememTo());
	p->printT();

	delete caretaker;
	delete p;
}

void main26_01()
{
	MememTo *memto = NULL;
	Person *p = new Person("����", 32);
	p->printT();

	//���� Person�����һ��״̬
	printf("---------\n");
	memto = p->createMememTo();
	p->setAge(42);
	p->printT();

	printf("��ԭ�ɵ�״̬\n" );
	p->setMememTo(memto);
	p->printT();

	delete memto;
	delete p;
}
void main()
{
	//main26_01();
	main26_02();
	system("pause");
	return ;
}