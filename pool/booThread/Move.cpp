#include "Define.h"
#include <vector>
#include <algorithm>
#include<functional>

void some_function(){}
void some_other_function(){}

//std::thread是可移动的，不可拷贝。在std::thread对象之间转移线程所有权使用sd::move函数。
void mainMove()
{
    std::thread t1(some_function);           // 1
    std::thread t2 = std::move(t1);            //t1转移到t2
    t1 = std::thread(some_other_function);     // 3 临时对象会隐式调用std::move转移线程所有权
    std::thread t3;                          // 4
    t3 = std::move(t2);                        // t2转移到t3
    //t1 = std::move(t3);                        // 6 赋值操作将使程序崩溃（t1已有线程，不可转移）
    if (t1.joinable())
    {
        t1.detach();                           //分离t1
    }
    t1 = std::move(t3);                        // 7
    if (t1.joinable())
    {
        t1.detach();
    }
    if (t2.joinable())
    {
        t2.detach();
    }
}

//因为临时对象会隐式调用std::move转移线程所有权
//所以线程的所有权可以在函数外进行转移。
std::thread f()
{
    return std::thread(some_function);
}
void some_other_function2(int){}
std::thread g()
{
    std::thread t(some_other_function2, 42);
    return t;
}
//所以可做参数
void f2(std::thread t){}
void g2()
{
    f2(std::thread(some_function));
    std::thread t(some_function);
    f2(std::move(t));
}


//利用线程可以转移的特性我们可以用容器来集中管理线程
void do_work(unsigned id)
{
    cout << id << endl;
}
void f3()
{
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 20; ++i)
    {
        threads.push_back(std::thread(do_work, i));
    }
    std::for_each(threads.begin(), threads.end(),
        std::mem_fn(&std::thread::join));
}
