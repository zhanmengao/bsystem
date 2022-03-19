#ifndef BOOKILL_H
#define BOOKILL_H

#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

//使用boost::this_thread::sleep_for()的中断点
void wait(int seconds)
{
    boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
}

void thread()
{
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

int mainKill()
{
    boost::thread t{thread};
    wait(3);
    t.interrupt();
    t.join();
}
#endif // BOOKILL_H
