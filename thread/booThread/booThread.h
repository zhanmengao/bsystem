#ifndef BOOTHREAD_H
#define BOOTHREAD_H

#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

//使用boost::thread
void wait(int seconds)
{
    boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
}

void threadMain()
{
    for (int i = 0; i < 5; ++i)
    {
        wait(1);
        std::cout << i << '\n';
    }
}

int mainBooThr()
{
    boost::thread t{threadMain};
    t.join();
	return 0;
}
#endif // BOOTHREAD_H
