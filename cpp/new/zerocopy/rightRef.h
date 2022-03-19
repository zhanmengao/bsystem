#pragma once
#include<memory>

//根据名字大概就可以猜到意思，左值引用就是对左值进行引用的类型，右值引用就是对右值进行引用的类型，
//他们都是引用，都是对象的一个别名，并不拥有所绑定对象的堆存，所以都必须立即初始化。
int rightRef()
{
	int a = 4;
	//int &&b = a; // error, a是左值
	int &&c = std::move(a); // ok
}