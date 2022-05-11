#ifndef BOOCANCEL2_H
#define BOOCANCEL2_H

#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

//使用disable_interruption禁止中断点
void wait(int seconds)
{
    boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
}

void thread()
{
    boost::this_thread::disable_interruption no_interruption;
    try
    {
        for (int i = 0; i < 5; ++i)
        {
            wait(1);
            std::cout << i << '\n';
        }
    }
    catch (boost::thread_interrupted&) {}
}

int mainCancel2()
{
    boost::thread t{thread};
    wait(3);
    t.interrupt();
    t.join();
}
#endif // BOOCANCEL2_H
