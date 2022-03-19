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


//�� ʹ�ö��壬��ʹ��ʵ��
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
	//��ʹ��ָ�룬��ʹ������
	//�۽�����ʵ��ϸ��������һ��ָ�뱳��
	std::tr1::shared_ptr<PersonImpl> pImpl;
};