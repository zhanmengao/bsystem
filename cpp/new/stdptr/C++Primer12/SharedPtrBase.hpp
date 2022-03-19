#pragma once
#include<string>
#include"Blob.hpp"
using namespace std;
/*
什么时候用shared_ptr：
1、程序不知道自己需要使用多少对象（容器）
2、程序不知道所需对象的准确类型
3、程序需要在多个对象间共享数据
*/
shared_ptr<MyClass> c1;					//不调用默认构造函数，保存一个空指针
shared_ptr<MyClass> c2(new MyClass("Shader c2"));
void CreateSharedMain()
{
	//shared_ptr<string> p2 = new string();	//error，shared_ptr构造函数为explicit
	if (c1 == nullptr)
	{
		c1.reset(new MyClass("c1"));
	}
	//如果p1不为空，检查它是否指向一个空string
	else if (c1 && c1->empty())
	{
		*c1 = "c1";
	}

	new MyClass("c3");									//!!!!!不会自动析构，内存泄漏

	shared_ptr<int> p3 = make_shared<int>(42);
	cout << "make_shared<int>(42)	" << *p3 << endl;

	shared_ptr<string> p4 = make_shared<string>(10, '9');
	cout << "make_shared<string>(10, '9')	" << *p4 << endl;

	shared_ptr<string> p5 = make_shared<string>();				//使用值初始化
	cout << "make_shared<string>()	" << *p5 << endl;
	{
		auto c5 = MyClass::factory("c5");
	}
	//离开作用域，c5被析构
}

void CopySharedMain()
{
	shared_ptr<MyClass> c4(new MyClass("c4"));
	c2 = c4;															//c2引用数为0，被析构

	shared_ptr<MyClass> pCpy(c2);				//拷贝一个智能指针，引用数++
	cout << "pCpy.use_count() " << pCpy.use_count() << endl;
	cout << "*pCpy " << *pCpy << endl;
	//检查是否是唯一指向它的指针
	if (!c2.unique())
	{
		c2.reset(new MyClass("reset"));
	}
	*c2 += "c2";

	auto c6 = MyClass::Clone(*c4);			//深拷贝一个c4
	*c6 = ("c6");
	//重定向
	c6.reset(new MyClass("c66"));							//c6被析构

	//shared_ptr<MyClass> c(c6.get());		//错误！！不能直接绑定智能指针get出来的内存

	swap(c6, c2);					//交换他们的指针
}

void ParameterShaderMain()
{
	auto c = MyClass::factory("c9");
	MyClass::ProcessShared(MyClass::factory("c10"));			//执行结束后c10被释放
	MyClass::ProcessShared(shared_ptr<MyClass>(c));
	MyClass::ProcessPtr(c.get());					//传递原生指针

}