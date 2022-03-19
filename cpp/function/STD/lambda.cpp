#include<iostream>
#include<memory>
using namespace std;

//[捕捉列表](参数) mutable -> 返回值类型 {函数体}
void mainLambda()
{
	int a = 5, b = 7;
	auto total = [](int x, int y)->int {return x + y; };			//接受两个参数
	cout << total(a, b) << endl;										//12

	auto fun1 = [=] {return a + b; };									//值传递捕捉父作用域变量
	cout << fun1() << endl;												//12

	auto fun2 = [&](int c) {b = a + c; a = 1; };					//省略了返回值类型，引用捕获所有
	fun2(3);    
	cout << a << " " << b << endl;
	a = 5; b = 7;																		//被修改后，重新赋值

	auto fun3 = [=, &b](int c) mutable {b = a + c; a = 1; };    //以值传递捕捉的变量，在函数体里如果要修改，要加mutaple,因为默认const修饰
	fun3(3);
	cout << a << " " << b << endl;
	a = 5; b = 7;																		 //被修改后，重新赋值

	auto fun4 = [=](int x, int y) mutable->int {a += x; b += y; return a + b; };
	int t = fun4(10, 20);
	cout << t << endl;
	cout << a << " " << b << endl; 
}

/*
Lambda函数与仿函数的关系
在C++11之前，STL中的一些算法需要使用一种函数对象-- - 仿函数(functor)；
其本质是重新定义和成员函数operator()，使其使用上很像普通函数，
其实，细心的我们已经发现，Lambda函数与仿函数似乎有一些默契。
仿函数以rate初始化,Lambda捕捉rate变量，参数传递上，两者一致。
事实上，仿函数就是实现Lambda函数一种方式，
编译器通常会把Lambda函数转换为一个放函数对象，但是仿函数的语法却给我们带来了很大的便捷。
在C++11中，Lambda函数被广泛使用，很多仿函数被取代。
*/
class Price
{
private:
	float _rate;
public:
	Price(float rate) :_rate(rate) {}
	float operator()(float price)
	{
		return price*(1 - _rate / 100);
	}
};

void mainLa2()
{
	float rate = 5.5f;

	Price c1(rate);
	auto c2 = [rate](float price)->float {return price*(1 - rate / 100); };

	float p1 = c1(3699);    //仿函数
	float p2 = c2(3699);    //Lambda函数
}
/*
Lambda与static inline函数
Lambda函数可以省略外部声明的static inline函数，其相当于一个局部函数。局部函数仅属于父作用域，
比起外部的static inline函数，或者是自定义的宏，Lambda函数并没有实际运行时的性能优势(但也不会差)
但是Lambda函数可读性更好。
父函数结束后，该Lambda函数就不再可用了，不会污染任何名字空间.

关于值传递捕捉和mutable
 上面提到过mutable 可以取消Lambda的常量属性，如果值传递想要在函数域内修改就要加mutable
*/
void mainLa3()
{
	int j = 12;
	auto by_val = [=] {return j + 1; };
	auto by_ref = [&] {return j + 1; };
	cout << by_val() << endl;   //13
	cout << by_ref() << endl;   //13
	j++;
	cout << by_val() << endl;   //13
	cout << by_ref() << endl;   //14
}
/*
上面的例子，j++了之后调用值传递结果依旧是12，原因是，值传递j被视为一个常量，一旦初始化，
就不会再修改（可以认为是一个和父作用域中j同名的常量），而再引用捕捉中，j仍然是父作用域中的值。
其实一个值传递的的Lambda转换为放函数后，会成为一个class的常量成员函数，
*/
class const_val_lambda
{
public:
	const_val_lambda(int v) :val(v) {}
public:
	//void operator()()const { val = 3; } //报错
private:
	int val;
};
/*
准确地讲，现有C++11标准中的lambda等价的是有常量operatorO的仿函数。
因此在使用捕捉列表的时候必须注意，按值传递方式捕捉的变量是lambda函数中不可更改的常量。
标准这么设计可能是源自早期STL算法一些设计上的缺陷（对仿函数没有做限制，从而导致一些设计不算特别良好的算法出错）。
而更一般地讲，这样的设计有其合理性，改变从上下文中拷贝而来的临时变量通常不具有任何意义。
绝大多数时候，临时变量只是用于lambda函数的输入，如果需要输出结果到上下文，
我们可以使用引用，或者通过让lambda函数返回值来实现。
此外，lambda函数的mutable修饰符可以消除其常量性，不过这实际上只是提供了一种语法上的可能性，
现实中应该没有多少需要使用mutable的lambda函数的地方。
大多数时候，我们使用默认版本的（非mutable）的lambda函数也就足够了。
*/

/*
Lambda函数与函数指针
Lambda函数并不是简单的函数指针类型，或者自定义类型；每个Lambda函数会产生一个闭包类型的临时对象(右值)。但是C++11允许Lambda函数向函数指针的转换，前提是：
Lambda没有捕捉任何变量
函数指针所示的函数原型，必须和Lambda有相同的调用方式
*/
#include <memory>
#include <functional>

class A
{

};

void(*testDeleter1)(A *) { [](A *) {} };							//函数指针形式
auto testDeleter2 = [](A *) {};										//lambda形式
std::function<void(A*)> testDeleter3 = [](A *) {};		//std::function形式

void main333()
{
	int a = 3, b = 4;

	auto total = [](int x, int y)->int {return x + y; };
	typedef int(*all)(int x, int y);
	typedef int(*one)(int x);

	all p;
	p = total;
	one q;
	//q = total;  //报错,参数不一致

	decltype(total) all_1 = total;
	//decltype(total) all_2 = p;  //报错，指针无法转换为Lambda

	A *pa1 = new A(), *pa2 = new A(), *pa3 = new A(), *pa4 = new A();


	std::unique_ptr< A, decltype(testDeleter1) >  uptr1(pa1, testDeleter1);
	std::unique_ptr< A, decltype(testDeleter2) >  uptr2(pa2, testDeleter2);
	std::unique_ptr< A, decltype(testDeleter3) >  uptr3(pa3, testDeleter3);
	std::unique_ptr< A, std::function<void(A*)> > uptr4(pa4, testDeleter3);
}
/*
[]是lambda的引出符，捕捉列表能够捕捉上下文中的变量，来供lambda函数使用：
[var] 表示以值传递方式捕捉变量var
[=] 表示值传递捕捉所有父作用域变量
[&var] 表示以引用传递方式捕捉变量var
[&] 表示引用传递捕捉所有父作用域变量
[this] 表示值传递方式捕捉当前的this指针
还有一些组合：
[=,&a] 表示以引用传递方式捕捉a,值传递方式捕捉其他变量
注意：
捕捉列表不允许变量重复传递，如：[=,a]、[&,&this]，会引起编译时期的错误
参数列表与普通函数的参数列表一致。如果不需要传递参数，可以联连同()一同【省略】。
mutable 可以取消Lambda的常量属性，因为Lambda默认是const属性；multable仅仅是让Lamdba函数体修改值传递的变量，但是修改后并不会影响外部的变量。
->返回类型如果是void时，可以连->一起【省略】，如果返回类型很明确，可以省略，让编译器自动推倒类型。
 函数体和普通函数一样，除了可以使用参数之外，还可以使用捕获的变量
*/

