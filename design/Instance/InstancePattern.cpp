#include<iostream>
#include <string>
using namespace std;

/*
单例模式：保证一个雷仅有一个实例，并提供一个访问它的全局访问点
*/
//懒汉式：需要的时候再new
class Dog
{
public:
	static Dog* getInstance()
	{
		if (_Ins == NULL)
		{
			_Ins = new Dog("大黄");
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

//饿汉式：不管要不要，直接new
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
Cat *Cat::_Ins = new Cat("白白");

int main()
{
	Dog *dog1 = Dog::getInstance();
	Dog *dog2 = Dog::getInstance();
	cout << dog1 << endl;
	cout << dog2 << endl;
	return 0;
}
