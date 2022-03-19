//
// Created by uzi on 12/8/20.
//

#ifndef LUSBDEVICE_USBUTILS_H
#define LUSBDEVICE_USBUTILS_H
#ifdef __cplusplus
#include<string>
#include<vector>
#endif

#include <libusb-1.0/libusb.h>
#ifdef __linux
#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include<pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<fcntl.h>
#else
#include<Windows.h>
#pragma comment(lib,"libusb-1.0.lib")
#pragma comment(lib,"legacy_stdio_definitions.lib")
#endif


#ifdef __linux
typedef struct USBDevice
{
    unsigned char   bLength;                  //该描述符结构体大小(18字节)
    unsigned char   bDescriptorType;          //描述符类型(本结构体中固定为0x01)
    unsigned short  bcdUSB;                   //USB版本号
    unsigned char   bDeviceClass;             //设备类代码(由USB官方分配)
    unsigned char   bDeviceSubClass;          //子类代码(由USB官方分配)
    unsigned char   bDeviceProtocol;          //设备协议代码(由USB官方分配)
    unsigned char   bMaxPacketSize0;          //端点0的最大包大小(有效大小为8,16,32,64)
    unsigned short  idVendor;                 //生产厂商编号(由USB官方分配)
    unsigned short  idProduct;                //产品编号(制造厂商分配)
    unsigned short  bcdDevice;                //设备出厂编号
    unsigned char   iManufacturer;            //设备厂商字符串索引
    unsigned char   iProduct;                 //产品描述字符串索引
    unsigned char   iSerialNumber;            //设备序列号字符串索引
    unsigned char   bNumConfigurations;       //当前速度下能支持的配置数量
} USB_DEVICE_DESCRIPTOR;

typedef struct USBConfig{
    unsigned char  bLength;                  //该描述符结构体大小
    unsigned char  bDescriptorType;          //描述符类型(本结构体中固定为0x02)
    unsigned short wTotalLength;             //此配置返回的所有数据大小
    unsigned char  bNumInterfaces;           //此配置的接口数量
    unsigned char  bConfigurationValue;      //Set_Configuration命令所需要的参数值
    unsigned char  iConfiguration;           //描述该配置的字符串的索引值
    unsigned char  bmAttributes;             //供电模式的选择
    unsigned char  MaxPower;                 //设备从总线提取的最大电流
} USB_CONFIGURATION_DESCRIPTOR;

typedef struct {
    unsigned char  bLength;                  //该描述符结构体大小
    unsigned char  bDescriptorType;          //描述符类型(本结构体中固定为0x03)
    unsigned short unicode[0];               //Unicode编码的字符串
} USB_STRING_DESCRIPTOR;

typedef struct USBInterface
{
    unsigned char  bLength;                   //该描述符结构大小
    unsigned char  bDescriptorType;           //接口描述符的类型编号(0x04)
    unsigned char  bInterfaceNumber;          //接口的编号
    unsigned char  bAlternateSetting;         //备用编号，可以理解为接口的配置号。一个接口只能一个配置有效？
    unsigned char  bNumEndpoints;             //该接口使用的端点数，不包括端点0
    unsigned char  bInterfaceClass;           //接口类型
    unsigned char  bInterfaceSubClass;        //接口子类型
    unsigned char  bInterfaceProtocol;        //接口遵循的协议
    unsigned char  iInterface;                //描述该接口的字符串索引值
} USB_INTERFACE_DESCRIPTOR;

typedef struct USBEndpoint{
    unsigned char  bLength;                 //端点描述符字节数大小(7个字节)
    unsigned char  bDescriptorType;         //端点描述符类型编号(0x05)
    unsigned char  bEndpointAddress;        //端点地址及输入输出属性
    unsigned char  bmAttributes;            //端点的传输类型属性
    unsigned short wMaxPacketSize;          //端点收、发的最大包大小
    unsigned char  bInterval;               //主机查询端点的时间间隔
} USB_ENDPOINT_DESCRIPTOR;

typedef struct
{
    unsigned short Length;
    unsigned short NumberOfPipesExpected;
    unsigned char InterfaceNumber;
    unsigned char AlternateSetting;
    unsigned short Padding;
    unsigned int NumberOfPipes;
} TS_USBD_INTERFACE_INFORMATION;

typedef struct
{
    unsigned short MaximumPacketSize;
    unsigned short Padding;
    unsigned int MaximumTransferSize;
    unsigned int PipeFlags;
} TS_USBD_PIPE_INFORMATION;

typedef struct
{
    unsigned short Size;
    unsigned short Padding;
    unsigned int UsbdStatus;
    unsigned int ConfigurationHandle;
    unsigned int NumInterfaces;
} TS_URB_SELECT_CONFIGURATION_RESULT;

typedef struct
{
    unsigned short Length;
    unsigned char InterfaceNumber;
    unsigned char AlternateSetting;
    unsigned char Class;
    unsigned char SubClass;
    unsigned char Protocol;
    unsigned char Padding;
    unsigned InterfaceHandle;
    unsigned NumberOfPipes;
} TS_USBD_INTERFACE_INFORMATION_RESULT;

typedef struct
{
    unsigned short MaximumPacketSize;
    unsigned char EndpointAddress;
    unsigned char Interval;
    unsigned PipeType;
    unsigned PipeHandle;
    unsigned MaximumTransferSize;
    unsigned PipeFlags;
} TS_USBD_PIPE_INFORMATION_RESULT;

USB_DEVICE_DESCRIPTOR ReadUsbDevice(const char* name);

enum USB_TYPE
{
    USB_AUDIO = 0x01,
    USB_CDC = 0x02,
    USB_HID = 0x03,
    USB_PHY = 0x05,
    USB_IMG = 0x06,
    USB_PRINT = 0x07,
    USB_STORAGE = 0x08,
    USB_HUB = 0x09,
    USB_CDC_DATA = 0x0A,
    USB_CARD = 0x0B,
    USB_SECURITY = 0x0D,
    USB_VEDIO = 0x0E,
    USB_HEALTHCARE = 0X0F,
    USB_AUDIO_VEDIO = 0X10,
    USB_BILLBOARD = 0X11,
    USB_BRIDGT = 0X12,
    USB_DIAGNOSTIC = 0XDC,
    USB_WIRELESS = 0XE0,
    USB_MISCELLANEOUS = 0XEF,
    USB_APPLICATION = 0XFE,
    USB_VENDOR = 0XFF,
};
class USBUtils;
class USB
{
    friend class USBUtils;
public:
    std::string GetDeviceDesc();
    std::string GetVIDPID();
    bool IsHID();
    bool IsMassStorage();
    int MaxPacketSize();
    bool isoTransfer();
private:
    USBDevice device;
    std::vector<USBConfig> config;
    std::vector<USBInterface> interfacevec;
    std::vector<USBEndpoint> endpoint;
};
#endif
#define CHAR_MAX_LEN 256
#define BUF_MAX_LEN INT_MAX
struct usbDevice
{
    char sn[CHAR_MAX_LEN];
    char product[CHAR_MAX_LEN];
    char name[CHAR_MAX_LEN];
    uint16_t pid;
    uint16_t vid;
};
struct UsbDevice
{
    std::string sn;
    std::string product;
    std::string name;
    uint16_t pid;
    uint16_t vid;
public:
    UsbDevice();
    UsbDevice(const usbDevice& obj);
    UsbDevice& operator=(const usbDevice& obj);
};
class USBUtils
{
public:
#ifdef __linux
    static std::string GetDeviceDesc(unsigned char   bDeviceClass);
    static USB ReadUsbDevice(const char* path);
#endif
    static std::string GetVIDPID(unsigned short idVendor,unsigned  short idProduct);
    static std::vector<UsbDevice> GetUSBList();
};

#endif //LUSBDEVICE_USBUTILS_H
