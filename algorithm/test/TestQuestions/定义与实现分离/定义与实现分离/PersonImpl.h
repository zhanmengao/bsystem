#pragma once

#include "Person.h"
class Date
{
private:
	unsigned long long mTimer;
};
class Address
{
private:
	unsigned int mAreaId;
};
class PersonImpl
{
public:
	PersonImpl(const string& name, const Date& date, const Address& add);
	std::string name() const;
	Date date() const;
	Address address() const;
protected:
private:
	Date mDate;
	std::string mName;
	Address mAddr;
};

