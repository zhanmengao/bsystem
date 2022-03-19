
#pragma once
#include <stdio.h>
#include <tchar.h>
// TODO: reference additional headers your program requires here
#include <string>
#include <memory>
using namespace std;


class Bir;
class Email;

class PersonInterface
{
public:
	static std::tr1::shared_ptr<PersonInterface> Create(const std::string& name, const Bir& date, const Email& addr);

	virtual ~PersonInterface() = 0;
	virtual std::string name() const = 0;
	virtual Bir date() const = 0;
	virtual Email address() const = 0;
};