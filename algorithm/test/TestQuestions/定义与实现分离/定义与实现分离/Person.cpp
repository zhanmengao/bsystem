// stdafx.cpp : source file that includes just the standard includes
// 定义和实现分离.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "Person.h"
#include "PersonImpl.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file


Person::Person(const string& name, const Date& date, const Address& addr)
	:pImpl(new PersonImpl(name, date, addr))
{
}
std::string Person::name() const
{
	return pImpl->name();
}
Date Person::date() const
{
	return pImpl->date();
}
Address Person::address() const
{
	return pImpl->address();
}