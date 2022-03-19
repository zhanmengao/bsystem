#include<iostream>

class A
{
public:
	void function3()
	{
		std::cout<<"hello world!"<<std::endl;			//C++ 优化：没有使用this指针，不会报错
		//flag = 0;
	}
	int flag;
};

int main()
{
	A* a = NULL;
	a->function3();
}
