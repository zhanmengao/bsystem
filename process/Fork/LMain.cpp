#include <iostream>

using namespace std;
#include"ChildProcess.h"

int main()
{
    char buffer[1024] = {0};
    OSProcess pro;
    while(1)
    {
        std::cin>>buffer;
        pro.SetStartPath("/bin/sh",buffer);
        pro.Start();
        pro.Wait();
    }

    return 0;
}
