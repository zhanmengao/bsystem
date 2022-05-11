#include <iostream>
#include"USBUtils.h"
#include <signal.h>
#define USB "/dev/bus/usb/001/005"
void Catch(int sig)
{
    printf("recv sig %d",sig);
}
int mainl()
{
    signal(SIGABRT,Catch);
    std::cout << "Hello, World!" << std::endl;
    auto info = ReadUsbDevice(USB);
    printf("USB VID:PID = %04X:%04X : %s\n", info.idVendor, info.idProduct,USBUtils::GetDeviceDesc(info.bDeviceClass).c_str());
    auto usb = USBUtils::ReadUsbDevice(USB);
    std::cout<<usb.GetDeviceDesc()<<usb.GetVIDPID()<<std::endl;
    return 0;
}
