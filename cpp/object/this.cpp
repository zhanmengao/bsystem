#include<iostream>

class A
{
public:
	void function3()
	{
		std::cout<<"hello world!"<<std::endl;			//C++ �Ż���û��ʹ��thisָ�룬���ᱨ��
		//flag = 0;
	}
	int flag;
};

int main()
{
	A* a = NULL;
	a->function3();
}
