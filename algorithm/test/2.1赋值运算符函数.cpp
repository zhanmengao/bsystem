#include<iostream>
#include <string>
using namespace std;

class CMyString{
public:
	//���غ���
	CMyString& operator=(const CMyString& obj)
	{
		//���
		if (this == &obj)
		{
			return *this;
		}
		if (this->space != nullptr)
		{
			delete[] space;
			space = nullptr;
		}
		space = new char[obj.length + 1];
		strcpy(space, obj.space);
		length = obj.length;
		return *this;
	}
private:
	char *space;
	int length;
};
