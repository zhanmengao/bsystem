#pragma once

int Auto()
{
	//普通；类型
	int a = 1, b = 3;
	auto c = a + b;					// c为int型


	const int i = 5;				 //const类型
	auto j = i; // 变量i是顶层const, 会被忽略, 所以j的类型是int
	auto k = &i; // 变量i是一个常量, 对常量取地址是一种底层const, 所以b的类型是const int*
	const auto l = i; //如果希望推断出的类型是顶层const的, 那么就需要在auto前面加上cosnt

					  //引用和指针类型
	int x = 2;
	int& y = x;
	auto z = y; //z是int型不是int& 型
	auto& p1 = y; //p1是int&型
	auto p2 = &x; //p2是指针类型int*
}


int func() { return 0; };
int Decltype()
{
	//普通类型
	decltype(func()) sum = 5; // sum的类型是函数func()的返回值的类型int, 但是这时不会实际调用函数func()
	int a = 0;
	decltype(a) b = 4; // a的类型是int, 所以b的类型也是int

					   //不论是顶层const还是底层const, decltype都会保留   
	const int c = 3;
	decltype(c) d = c; // d的类型和c是一样的, 都是顶层const
	int e = 4;
	const int* f = &e; // f是底层const
	decltype(f) g = f; // g也是底层const

					   //引用与指针类型
					   //1. 如果表达式是引用类型, 那么decltype的类型也是引用
	const int i = 3, &j = i;
	decltype(j) k = 5; // k的类型是 const int&

					   //2. 如果表达式是引用类型, 但是想要得到这个引用所指向的类型, 需要修改表达式:
	int i = 3, &r = i;
	decltype(r + 0) t = 5; // 此时是int类型

						   //3. 对指针的解引用操作返回的是引用类型
	//int i = 3, j = 6, *p = &i;
	//decltype(*p) c = j; // c是int&类型, c和j绑定在一起

						//4. 如果一个表达式的类型不是引用, 但是我们需要推断出引用, 那么可以加上一对括号, 就变成了引用类型了
	int i = 3;
	decltype((i)) j = i; // 此时j的类型是int&类型, j和i绑定在了一起
}