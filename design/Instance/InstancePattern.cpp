#include<iostream>
#include <string>
using namespace std;

/*
����ģʽ����֤һ���׽���һ��ʵ�������ṩһ����������ȫ�ַ��ʵ�
*/
//����ʽ����Ҫ��ʱ����new
class Dog
{
public:
	static Dog* getInstance()
	{
		if (_Ins == NULL)
		{
			_Ins = new Dog("���");
		}
		return _Ins;
	}
private:
	Dog(string name)
	{
		this->name = name;
	}
private:
	static Dog *_Ins;
	string name;
};
Dog *Dog::_Ins = NULL;

//����ʽ������Ҫ��Ҫ��ֱ��new
class Cat
{
public:
	static Cat*  getInstance()
	{
		return _Ins;
	}
private:
	Cat(string name)
	{
		this->name = name;
	}
protected:
private:
	static Cat *_Ins;
	string name;
};
Cat *Cat::_Ins = new Cat("�װ�");

int main()
{
	Dog *dog1 = Dog::getInstance();
	Dog *dog2 = Dog::getInstance();
	cout << dog1 << endl;
	cout << dog2 << endl;
	return 0;
}
