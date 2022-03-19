#include <iostream>
#include"FileWatch.h"
#include"DirWatch.h"
int main()
{
    FileListener listener;
    for(int i = 1;i<=8;i++)
    {
        char pathBuffer[64] = {0};
        sprintf(pathBuffer, "/dev/bus/usb/%03d", i);
        listener.AddListen(pathBuffer);
    }
    bool bRet = listener.AddListen("/home/uzi");
    std::vector<notifyInfo> vec;

    while(1)
    {
        vec.clear();
        //listener.Read(vec);
        for(int i = 0;i<vec.size();i++)
        {
            auto& info = vec[i];
            cout<<info.name<<endl;
        }

        DirListener dirlistener("/home/uzi");
        vec.clear();
        dirlistener.Read(vec);
        for(int i = 0;i<vec.size();i++)
        {
            auto& info = vec[i];
            cout<<info.name;
            cout<<endl;
        }
    }
    return 0;
}
