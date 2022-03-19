#include <iostream>
#include"booThread.h"
using namespace std;

int main()
{
    //监测线程ID和可用处理器的数目
    std::cout << boost::this_thread::get_id() << '\n';
    std::cout << boost::thread::hardware_concurrency() << '\n';
    mainBooThr();
    return 0;
}
