#include "PersonImpl.h"

PersonImpl::PersonImpl(const string& name, const Date& date, const Address& add)
{

}
std::string PersonImpl::name() const
{
	return mName;
}
Date PersonImpl::date() const
{
	return mDate;
}
Address PersonImpl::address() const
{
	return mAddr;
}