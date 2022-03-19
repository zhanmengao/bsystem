#include <iostream>
#include"bLog.hpp"
using namespace std;

int main()
{
    LogManager log;
    log.SetPath("test");
    log.SetWriteStdout(true);
    log.LogDaily("OKOK");
    return 0;
}
