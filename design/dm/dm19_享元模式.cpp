
#include <iostream>
using namespace std;
#include "string"
#include "map"
/*
Flyweight模式也叫享元模式，是构造型模式之一，它通过与其他类似对象共享数据来减小内存占用。
*/
class Person
{
public:
	Person(string name, int age)
	{
		this->m_name = name;
		this->age = age;
	}
	virtual void printT() = 0;

protected:
	string	m_name;
	int		age;
};

class Teacher : public Person
{
public:
	Teacher(string name, int age, string id) : Person(name, age)
	{
		this->m_id = id;
	}
	void printT()
	{
		cout << "name:" << m_name << " age:" << age << " m_id:" << m_id << endl;
 	}
protected:
private:
	string	m_id;
};


//完成 老师结点 存储

class FlyWeightTeacherFactory 
{
public:
	FlyWeightTeacherFactory()
	{
		map1.clear();
	}

	~FlyWeightTeacherFactory()
	{
		while ( !map1.empty())
		{
			Person *tmp = NULL;
			map<string, Person *>::iterator it = map1.begin();
			tmp = it->second;
			map1.erase(it); //把第一个结点 从容器中删除
			delete tmp;
		}
	}

	Person * GetTeacher(string id)
	{
		Person *tmp = NULL;
		map<string, Person *>::iterator it ;
		it = map1.find(id);
		if (it == map1.end()) //没有找到
		{
			string	tmpname;
			int		tmpage;
			cout << "\n请输入老师name:";
			cin >> tmpname;

			cout << "\n请输入老师age:";
			cin >> tmpage;

			tmp = new Teacher(tmpname, tmpage, id);
			map1.insert(pair<string, Person*>(id, tmp) );
		}
		else
		{
			tmp = it->second;
		}
		return tmp;
	}
private:
	map<string, Person *> map1;

};

void main()
{
	Person *p1 = NULL;
	Person *p2 = NULL;
	FlyWeightTeacherFactory *fwtf = new FlyWeightTeacherFactory;
	p1 = fwtf->GetTeacher("001");
	p1->printT();

	p2 = fwtf->GetTeacher("001");
	p2->printT();

	delete fwtf;
	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}