#include <windows.h>
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

int main()
{
    string strMapName("ShareMemory");                // 内存映射对象名称
    string strComData("This is common data!");        // 共享内存中的数据
    LPVOID pBuffer;                                    // 共享内存指针

    // 首先试图打开一个命名的内存映射文件对象
    HANDLE hMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, strMapName.c_str());
    if (NULL == hMap)
    {    // 打开失败，创建之
        hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE,
                                   NULL,
                                   PAGE_READWRITE,
                                   0,
                                   strComData.length()+1,
                                   strMapName.c_str());
        // 映射对象的一个视图，得到指向共享内存的指针，设置里面的数据
        pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        strcpy((char*)pBuffer, strComData.c_str());
        cout << "写入共享内存数据：" << (char *)pBuffer << endl;
    }
    else
    {    // 打开成功，映射对象的一个视图，得到指向共享内存的指针，显示出里面的数据
        pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        cout << "读取共享内存数据：" << (char *)pBuffer << endl;
    }

    getchar();            // 注意，进程关闭后，所有句柄自动关闭，所以要在这里暂停

    // 解除文件映射，关闭内存映射文件对象句柄
    ::UnmapViewOfFile(pBuffer);
    ::CloseHandle(hMap);
    system("pause");
    return 0;
}
