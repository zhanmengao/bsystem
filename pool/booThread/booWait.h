#ifndef BOOWAIT_H
#define BOOWAIT_H
#include <boost/thread.hpp>
#include <boost/thread/scoped_thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

//使用boost::scoped_thread等待线程
void wait(int seconds)
{
  boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
}

void thread()
{
  for (int i = 0; i < 5; ++i)
  {
    wait(1);
    std::cout << i << '\n';
  }
}

int mainWait()
{
  boost::scoped_thread<> t{boost::thread{thread}};
}
#endif // BOOWAIT_H
