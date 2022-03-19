#pragma once
#include<typeinfo>
#include <iostream>
using namespace std;

class Flyable                       // �ܷɵ�
{
public:
	virtual void takeoff() = 0;     // ���
	virtual void land() = 0;        // ����
};
class Bird : public Flyable         // ��
{
public:
	void foraging() 
	{ 
		cout << "С����ʳ" << endl;
	}           // ��ʳ
	virtual void takeoff() { }
	virtual void land() {  }
	virtual ~Bird() {}
};
class Plane : public Flyable        // �ɻ�
{
public:
	void carry() { }              // ����
	virtual void takeoff() { }
	virtual void land() { }
};

//class type_info
//{
//public:
//	const char* name() const;
//	bool operator == (const type_info & rhs) const;
//	bool operator != (const type_info & rhs) const;
//	int before(const type_info & rhs) const;
//	virtual ~type_info();
//private:
//};

void doSomething(Flyable *obj)                 // ��Щ����
{
	obj->takeoff();

	cout << typeid(*obj).name() << endl;        // �������������ͣ�"class Bird" or "class Plane"��

	if (typeid(*obj) == typeid(Bird))            // �ж϶�������
	{
		Bird *bird = dynamic_cast<Bird *>(obj); // ����ת��
		bird->foraging();
	}

	obj->land();
}

int main() 
{
	Bird *b = new Bird();
	doSomething(b);
	delete b;
	b = nullptr;
	return 0;
}