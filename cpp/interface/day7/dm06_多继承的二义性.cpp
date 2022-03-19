
#include <iostream>
using namespace std;

class  B
{
public:
	int b;
protected:
private:
};

class  B1 : virtual public B
{
public:
	int b1;
protected:
private:
};

class  B2 : virtual public B
{
public:
	int b2;
protected:
private:
};

class  C : public B1, public B2
{
public:
	int c;
protected:
private:
};



void main601()
{
	 C myc;
	 myc.c = 10;
	 myc.b = 100;//二义性  error C2385: 对“b”的访问不明确
}