#include "PersonInterface.h"
#include "PersonImpl.h"

class Bir
{
private:
	unsigned long long mTimer;
};
class Email
{
private:
	unsigned int mAreaId;
};
class RealPerson:public PersonInterface
{
public:
	RealPerson(const std::string& name, const Bir& date, const Address& addr);
public:
	virtual ~RealPerson();
	virtual std::string name() const;
	virtual Bir date() const;
	virtual Email address() const;
protected:
private:
	string mName;
	Bir mBir;
	Email mEamil;
};