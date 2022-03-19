#include<iostream>
/*外观模式的效果：

1、对客户屏蔽了子系统组件，因而减少了客户端处理对象的数目，使得子系统使用起来更加方便。

2、实现了子系统和客户端之间的松耦合关系，而子系统的内部各个部件通常都是紧偶尔的。

3、如果客户端需要，它不限制客户端使用子系统类，因此可以再系统的易用性和通用性之间作选择。
*/

/*Facade 为外观类，处理客户端的请求，外观类屏蔽了客户端与各子系统，
使得客户端不需要知道子系统的实现细节，各子系统通过外观类统一对外提供
比如说有个VC的编译功能，通过VC的BUILD对外提供，我们不需要知道内部
标识符解析，符号解析等等的细节.
*/


class Class1
{
public:
	void MethodA() { std::cout << "class 1 MethodA\n"; };

};

class Class2
{
public:
	void MethodB() { std::cout << "class 2 MethodB\n"; };

};

class Class3
{
public:
	void MethodC() { std::cout << "class 3 MethodC\n"; };

};

class Class4
{
public:
	void MethodD() { std::cout << "class 4 MethodD\n"; };

};

class Class5
{
public:
	void MethodE() { std::cout << "class 5 MethodE\n"; };

};

class Facade
{
public:
	Facade();
	~Facade();
	void FunctionA();
	void FuncTionB();
private:
	Class1* pm_class1;
	Class2* pm_class2;
	Class3* pm_class3;
	Class4* pm_class4;
	Class5* pm_class5;
};


Facade::Facade()
{
	pm_class1 = new Class1();
	pm_class2 = new Class2();
	pm_class3 = new Class3();
	pm_class4 = new Class4();
	pm_class5 = new Class5();
}

Facade::~Facade()
{
	delete pm_class1;
	delete pm_class2;
	delete pm_class3;
	delete pm_class4;
	delete pm_class5;
}

//功能A
void Facade::FunctionA()
{
	pm_class1->MethodA();
	pm_class2->MethodB();
	pm_class3->MethodC();
}

//功能B
void Facade::FuncTionB()
{
	pm_class1->MethodA();
	pm_class2->MethodB();
	pm_class4->MethodD();
	pm_class5->MethodE();
}


int main()
{
	/*通过外观类，客户不需要所需功能的内部各个类如何处理请求，
	如何实现这个功能，内部类的修改不影响客户端的代码，客户端不需要重新实现代码*/

	Facade facade;
	facade.FunctionA();//客户需要的功能A
	facade.FuncTionB();//客户需要的功能B
	return 0;
}