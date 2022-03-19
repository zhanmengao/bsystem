// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
#include <string>
#include <memory>
using namespace std;


//② 使用定义，不使用实现
class PersonImpl;
class Date;
class Address;
class Person
{
public:
	Person(const string& name, const Date& date,const Address& add);
	std::string name() const;
	Date date() const;
	Address address() const;
private:
	//①使用指针，不使用引用
	//③将对象实现细节隐藏于一个指针背后
	std::tr1::shared_ptr<PersonImpl> pImpl;
};