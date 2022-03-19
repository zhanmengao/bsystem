#include<iostream>
#include <string>
using namespace std;

//¶öººÊ½
class Singleton{
private:
	Singleton()
	{

	}
private:
	static Singleton *instance;
public:
	Singleton& getInstance()
	{
		return *instance;
	}
};
Singleton* Singleton::instance = new Singleton;
