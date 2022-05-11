//
// Created by uzi on 12/9/20.
//
#include"USBUtils.h"

#ifdef __linux
USB_DEVICE_DESCRIPTOR ReadUsbDevice(const char* name)
{
    printf("[libusb_udevman__FUNCTION]:%s\n", __FUNCTION__);

    USB_DEVICE_DESCRIPTOR info;
    unsigned char buffer[4096];
    int fd = open(name, O_RDONLY);
    if (fd < 0)
    {
        perror(name);
        return info;
    }

    int descriptorLength = read(fd, buffer, 4096);
    if (descriptorLength < 36)
    {
        perror(name);
        close(fd);
        return info;
    }
    memcpy(&info,buffer,descriptorLength);
    close(fd);
    return info;
}
std::string USB::GetDeviceDesc()
{
    if(device.bDeviceClass != 0x00)
    {
        return USBUtils::GetDeviceDesc(device.bDeviceClass);
    }
    else
    {
        for(auto it = interfacevec.begin();it!=interfacevec.end();it++)
        {
            if(it->bInterfaceClass!=0x00)
            {
                return USBUtils::GetDeviceDesc(it->bInterfaceClass);
            }
        }
    }
}
std::string USB::GetVIDPID()
{
    return USBUtils::GetVIDPID(device.idVendor,device.idProduct);
}
bool USB::IsHID()
{
    bool humanInterfaceDevice = 1;
    for(auto it = interfacevec.begin();it!=interfacevec.end();it++)
    {
        if (it->bInterfaceClass != 0x08)
            humanInterfaceDevice = 0; //只要有一个 interface 不是储存器就认为不是储存器
            break;
    }
    return humanInterfaceDevice;
}
bool USB::IsMassStorage()
{
    bool massStorage = 1;
    for(auto it = interfacevec.begin();it!=interfacevec.end();it++)
    {
        if (it->bInterfaceClass != 0x03)
            massStorage = 0; //只要有一个 interface 不是 HID 就认为不是 HID
            break;
    }
    return massStorage;
}
int USB::MaxPacketSize()
{
    int wMaxPacketSize = 0;
    for(auto it = endpoint.begin();it!=endpoint.end();it++)
    {
        if (it->wMaxPacketSize > wMaxPacketSize)
        {
            wMaxPacketSize = it->wMaxPacketSize; //记下端点的最大包大小，稍后用来判断U盾和储存器。
        }
    }
    return wMaxPacketSize;
}
bool USB::isoTransfer()
{
    int isoTransfer = 0;
    for(auto it = endpoint.begin();it!=endpoint.end();it++)
    {
        if ((it->bmAttributes & 3) == 1)
        {
            isoTransfer = 1; //是否有同步传输端点
        }
    }
    return isoTransfer;
}
std::string USBUtils::GetDeviceDesc(unsigned char   bDeviceClass)
{
    std::string ret;
    switch (bDeviceClass)
    {
        case USB_AUDIO:ret = "Audio 音频设备";break;
        case USB_CDC:ret = "Communications&CDC 通信设备";break;
        case USB_HID:ret = "HID(Human Interface Device)人机接口设备";break;
        case USB_PHY:ret = "Physical 物理设备";break;
        case USB_IMG:ret = "Image 图像设备";break;
        case USB_PRINT:ret = "Printer 打印机";break;
        case USB_STORAGE:ret = "Mass Storage 大容量存储";break;
        case USB_HUB:ret = "Hub 集线器";break;
        case USB_CDC_DATA:ret = "CDC-Data 通信设备";break;
        case USB_CARD:ret = "Smart Card 智能卡";break;
        case USB_SECURITY:ret = "Content Security 内容安全设备";break;
        case USB_VEDIO:ret = "Video 视频设备";break;

        case USB_HEALTHCARE:ret = "Personal Healthcare 个人健康设备";break;
        case USB_AUDIO_VEDIO:ret = "Audio/Video Devices 声音/视频设备";break;
        case USB_BILLBOARD:ret = "Billboard Device Class 广播牌设备";break;
        case USB_BRIDGT:ret = "USB Type-C Bridge Class 桥接器";break;
        case USB_DIAGNOSTIC:ret = "Diagnostic Device 诊断装置";break;
        case USB_WIRELESS:ret = "Wireless Controller 无线控制器";break;
        case USB_MISCELLANEOUS:ret = "Miscellaneous 其他设备";break;
        case USB_APPLICATION:ret = "Application Specific 具体应用";break;
        case USB_VENDOR:ret = "Vendor Specific 供应商特定设备";break;
    }
    return ret;
}

USB USBUtils::ReadUsbDevice(const char* path)
{
    USB usb;
    unsigned char buffer[4096] ={ 0 };
    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        perror(path);
        return usb;
    }

    int descriptorLength = read(fd, buffer, 4096);
    if (descriptorLength < 36)
    {
        perror(path);
        close(fd);
        return usb;
    }
    close(fd);
    memcpy(&usb.device,buffer,sizeof(usb.device));


    int offset = 18;
    int isoTransfer = 0;
    int massStorage = 1;
    int wMaxPacketSize = 0;
    int humanInterfaceDevice = 1;
    while (offset < descriptorLength - 2)
    {
        if (buffer[offset + 1] == 0x04)
        {
            USB_INTERFACE_DESCRIPTOR* interface = (USB_INTERFACE_DESCRIPTOR*)(buffer + offset);
            if (interface->bInterfaceClass != 0x08)
                massStorage = 0; //只要有一个 interface 不是储存器就认为不是储存器
            if (interface->bInterfaceClass != 0x03)
                humanInterfaceDevice = 0; //只要有一个 interface 不是 HID 就认为不是 HID

            USB_INTERFACE_DESCRIPTOR info = *interface;
            usb.interfacevec.push_back(info);
        }
        else if (buffer[offset + 1] == 0x05)
        {
            USB_ENDPOINT_DESCRIPTOR* endPoint = (USB_ENDPOINT_DESCRIPTOR*)(buffer + offset);
            if (endPoint->wMaxPacketSize > wMaxPacketSize)
            {
                wMaxPacketSize =
                        endPoint->wMaxPacketSize; //记下端点的最大包大小，稍后用来判断U盾和储存器。
            }
            if ((endPoint->bmAttributes & 3) == 1)
            {
                isoTransfer = 1; //是否有同步传输端点
            }
            USB_ENDPOINT_DESCRIPTOR info = *endPoint;
            usb.endpoint.push_back(info);
        }
        else if (buffer[offset + 1] == 0x02)
        {
            USB_CONFIGURATION_DESCRIPTOR* config = (USB_CONFIGURATION_DESCRIPTOR*)(buffer + offset);
            usb.config.push_back(*config);
        }
        offset += buffer[offset];
    }

    if (isoTransfer)
        //return 0; //同步传输暂时不支持
        if (humanInterfaceDevice)
            //return 0; // HID不被重定向
            // if((massStorage) && (wMaxPacketSize >= 512)) return 0;
            // //储存期可以文件重定向。根据观察，U盾的端点的包大小通常是16、32、64，储存器的是512。
            return usb;
}

std::string USBUtils::GetVIDPID(unsigned short idVendor,unsigned  short idProduct)
{
    char name[64] = {0};
    sprintf(name,"%04X:%04X", idVendor, idProduct);
    return name;
}
#endif


ssize_t lsUsbDevice(struct usbDevice *st)
{
    ssize_t usb_list_len = 0, idx = 0;
    struct libusb_device **list = NULL;
    struct libusb_device *dev = NULL;
    struct libusb_device_descriptor desc = { 0 };
    struct libusb_device_handle *usb_p = NULL;
    char buf[CHAR_MAX_LEN] = { 0 };

    //get the list
    usb_list_len = libusb_get_device_list(NULL, &list);
    if (usb_list_len == 0)
    {
        perror("can't find usb list");
        return 0;
    }
    //access the members of the list
    for (idx = 0; idx != usb_list_len; idx++)
    {
        memset(&desc, sizeof(struct libusb_device_descriptor), 0);
        if (libusb_get_device_descriptor(list[idx], &desc) != 0)
        {
            perror("can't find usb list information");
            return 0;
        }
        st[idx].pid = desc.idProduct;
        st[idx].vid = desc.idVendor;
        //copy usb device's information to the st
        usb_p = libusb_open_device_with_vid_pid(NULL, desc.idVendor, desc.idProduct);
        if (usb_p)
        {
            libusb_get_string_descriptor_ascii(usb_p, desc.iProduct, (unsigned char*)st[idx].product, CHAR_MAX_LEN);
            libusb_get_string_descriptor_ascii(usb_p, desc.iSerialNumber, (unsigned char*)st[idx].sn, CHAR_MAX_LEN);
            libusb_close(usb_p);
            usb_p = NULL;
        }
    }
    //return the lenght of list
    return usb_list_len;
}

UsbDevice::UsbDevice()
{

}
UsbDevice::UsbDevice(const usbDevice& obj)
{
    sn = obj.sn;
    product = obj.product;
    name = obj.name;
    pid = obj.pid;
    vid = obj.vid;
}
UsbDevice& UsbDevice::operator=(const usbDevice& obj)
{
    sn = obj.sn;
    product = obj.product;
    name = obj.name;
    pid = obj.pid;
    vid = obj.vid;
    return *this;
}
std::vector<UsbDevice> USBUtils::GetUSBList()
{
    std::vector<UsbDevice> vec;
    struct usbDevice st[CHAR_MAX_LEN] = { 0 };
    ssize_t usb_list_len = 0;
    int iret = 0;
    iret = libusb_init(NULL);
    usb_list_len = lsUsbDevice(st);
    for (int i = 0; i < usb_list_len; i++)
    {
        vec.push_back(st[i]);
    }
    libusb_exit(NULL);
    return vec;
}
