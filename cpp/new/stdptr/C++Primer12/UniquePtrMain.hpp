#pragma once
#include<memory>
#include"Blob.hpp"

typedef void(*pDeleteMyClass)(MyClass* pClass);
//删除器
void DeleteMyClass(MyClass* pClass)
{
	printf("Call DeleteMyClass\n");
	delete pClass;
}
unique_ptr<MyClass> p1(new MyClass("Unique p1"));
void CreateUniqueMain()
{
	unique_ptr<double> d1;					//可以指向一个double，空
	unique_ptr<double> d2(new double(42));

	//提供一个删除器
	unique_ptr<MyClass, pDeleteMyClass> p(new MyClass("pDelete"), DeleteMyClass);
}
void CopyUniqueMain()
{
	//unique_ptr<MyClass> p2(p1);			//error，不支持拷贝构造
	unique_ptr<MyClass> p3;
	//p3 = p1;					//error，不支持赋值

	//转移
	//release()：放弃对指针的控制权，返回指针，并将u置空
	unique_ptr<MyClass> p4(p1.release());
	p3.reset(p4.release());
	p3 = nullptr;				//释放p3指向的对象，并将p3置空

	p4.reset(new MyClass("p4"));
	p4.reset(nullptr);				//等价于 = nullptr

	p4.reset(new MyClass("p5"));
	auto pMyP5 = p4.release();					//智能指针不再管理p5，需要自行清理
	if (pMyP5 != nullptr)
	{
		delete pMyP5;
		pMyP5 = nullptr;
	}
}

void ParameterUniqueMain()
{
	auto pClone = MyClass::UClone(*p1);				//深拷贝一个p

}
