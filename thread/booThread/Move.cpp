#include "Define.h"
#include <vector>
#include <algorithm>
#include<functional>

void some_function(){}
void some_other_function(){}

//std::thread�ǿ��ƶ��ģ����ɿ�������std::thread����֮��ת���߳�����Ȩʹ��sd::move������
void mainMove()
{
    std::thread t1(some_function);           // 1
    std::thread t2 = std::move(t1);            //t1ת�Ƶ�t2
    t1 = std::thread(some_other_function);     // 3 ��ʱ�������ʽ����std::moveת���߳�����Ȩ
    std::thread t3;                          // 4
    t3 = std::move(t2);                        // t2ת�Ƶ�t3
    //t1 = std::move(t3);                        // 6 ��ֵ������ʹ���������t1�����̣߳�����ת�ƣ�
    if (t1.joinable())
    {
        t1.detach();                           //����t1
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

//��Ϊ��ʱ�������ʽ����std::moveת���߳�����Ȩ
//�����̵߳�����Ȩ�����ں��������ת�ơ�
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
//���Կ�������
void f2(std::thread t){}
void g2()
{
    f2(std::thread(some_function));
    std::thread t(some_function);
    f2(std::move(t));
}


//�����߳̿���ת�Ƶ��������ǿ��������������й����߳�
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
