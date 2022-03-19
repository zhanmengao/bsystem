#include "RealPerson.h"

std::tr1::shared_ptr<PersonInterface> PersonInterface::Create(const std::string& name, const Bir& date, const Email& addr)
{
	return std::tr1::shared_ptr<PersonInterface>(new RealPerson(name, date, addr));
}


RealPerson::~RealPerson()
{

}
std::string RealPerson::name() const
{
	return mName;
}
Bir RealPerson::date() const
{
	return mBir;
}
Email RealPerson::address() const
{
	return mEamil;
}