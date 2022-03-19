
#include <iostream>
using namespace std;


class Test3
{
public:
	void init(int _a, int _b)
	{
		a = _a;
		b = _b;
	}

protected:
private:
	int a;
	int b;
};


void main301()
{

	//类没有提供构造函数,c++编译器会自动给你提供一个默认的构造函数
	//类没有提供构造函数 copy构造构造函数, c++编译器会自动给程序员提供一个 默认的copy构造函数  = 
	Test3 t1;
	int a = 10;
	int b = 20;
	t1.init(a, b);

	Test3 tArray[3];
	tArray[0].init(1, 2);
	tArray[1].init(1, 2);
	tArray[2].init(1, 2);

	Test3 t21;
	t21.init(1, 2);
	Test3 t22;
	t22.init(1, 2);
	Test3 t23;
	t23.init(1, 2);

	//在这种场景之下 显示的初始化方案 显得很蹩脚
	Test3 tArray2[3] = { t21, t22, t23 };

	//在这种场景之下,满足不了,编程需要
	Test3 tArray3[1999] = { t21, t22, t23 };


	cout << "hello..." << endl;
	system("pause");
	return;
}