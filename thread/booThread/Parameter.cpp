#include "Define.h"
#ifdef __linux
#include<unistd.h>
#define Sleep(n) sleep(n/1000)
#else
#include <Windows.h>
#endif
/*
默认情况下，即使我们线程函数的参数是引用类型，参数会先被拷贝到线程空间，然后被线程执行体访问。上面的线程空间为线程能够访问的内部内存。
*/
void f(int i, std::string const& s)
{
	cout << i << "call,str = " << s << endl;
}

//即使f的第二个参数是引用类型，字符串字面值"hello"还是被拷贝到线程t空间内，然后被转换为std::string类型。
void oopsConst(int some_param)
{
    std::thread t(std::ref(f), some_param, "hello");
	t.detach();
}
//但是在下面这种参数为指向自动变量的指针的情况下就很容易出错。在这种情况下，指针变量buffer将会被拷贝到线程t空间内，
//这个时候很可能函数oopsError结束了，buffer还没有被转换为std::string，线程函数拿到的就是一个已经被析构的栈变量
void oopsError(int some_param)
{
	char buffer[1024];
	sprintf(buffer, "%i", some_param);
    std::thread t(std::ref(f), some_param, buffer);           //隐式转换，在线程函数被调用才发生构造，不安全。
	t.detach();
}
void not_oops(int some_param)
{
	char buffer[1024];
	sprintf(buffer, "%i", some_param);
    std::thread t(std::ref(f), some_param, std::string(buffer));           //手动转换，这样传进去的就是string变量，安全
	t.detach();
}

/*
由于上面所说，进程传参时，参数都会被进行一次拷贝。
所以即使我们将进程函数参数设为引用，也只是对这份拷贝的引用，而非原数据的引用
我们对参数的操作并不会改变其传参之前的值。
*/
void display_status(int &w,string& data)
{
	cout << "w.path = " << &w << "      w =" << w << endl;
	cout <<"data.path = "<< &data << "     data ="<< data<<endl;
}
//发生拷贝，用引用接到的只是拷贝的引用
void PthreadMain(int &w, string& data)
{
	cout << "child------------------------------------"<<endl;
	display_status(w, data);
	w++;
	data = "222222222";
	Sleep(5000);
}
void oops_error_ref(int w)
{
	string data = "father";
    std::thread t(std::ref(PthreadMain), w, data);         //data发生拷贝，不再是原来的变量
	t.join();
	data += to_string(w);
	cout << "oops_error_ref------------------------------------" << endl;
	display_status(w, data);
}

//解决方法：用std::ref传递引用
void oops_ref(int w)
{
	string data = "father";
    std::thread t(std::ref(PthreadMain), std::ref(w), std::ref(data));
	t.join();
	cout << "oops_ref------------------------------------" << endl;
	display_status(w, data);
}

/*
对于可以移动不可拷贝的参数，譬如std::unqiue_ptr对象，如果源对象是临时的，移动操作是自动执行的；
如果源对象是命名变量，必须显式调用std::move函数。
*/
void PthreadMovePtr(int &w, std::unique_ptr<string>& data)
{
	cout << "child------------------------------------" << endl;
	display_status(w, *data);
	w++;
	*data = "222222222";
	Sleep(5000);
}
void oops_move(int w)
{
	std::unique_ptr<string> p(new string);
	*p = "o00";
    std::thread t(std::ref(PthreadMovePtr), w, std::move(p));
	t.join();
	cout << "oops_move------------------------------------" << endl;
	display_status(w, *p);       //此刻p已经被转移走了
}

void mainPara()
{
	oopsConst(1);
	oopsError(2);
	not_oops(3);
	Sleep(3000);
	oops_error_ref(4);
	cout << endl;
	Sleep(1000);
	oops_ref(5);
	cout << endl;
	Sleep(1000);
	oops_move(6);
}
