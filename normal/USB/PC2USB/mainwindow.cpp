#include "mainwindow.h"
#include<string>
#include<string.h>
#include<stdio.h>
#include<QList>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    USBList = new QComboBox();
    QWidget * main = new QWidget();
    layout = new QVBoxLayout();
    setCentralWidget(main);
    main->setLayout(layout);
    openBtn = new QPushButton("open");
    sendBtn = new QPushButton("send");
    layout->addWidget(USBList);
    layout->addWidget(openBtn);
    layout->addWidget(sendBtn);
    connect(openBtn,&QPushButton::clicked,this,&MainWindow::OnClickOpenUSB);
    connect(sendBtn,&QPushButton::clicked,this,&MainWindow::OnClickSend);

    int r;
    ssize_t cnt;
    //libusb_context *ctx=nullptr; //context 上下文
    libusb_device **devs; //devices
    int i=0;


    r=libusb_init(&ctx); //init 初始化libusb
    if(r<0)
    {
        qDebug("failed to init libusb");
        return;
    }

    cnt = libusb_get_device_list(nullptr, &devs); //获取设备列表
    if (cnt < 0)
    {
        qDebug("failed to get device list");  //fail to get device_list
        return;
    }

    //遍历查找你要的设备
    while ((dev = devs[i++]) != nullptr)
    {
        struct libusb_device_descriptor desc;  //设备信息描述符

        r= libusb_get_device_descriptor(dev, &desc); //获取设备信息描述符
        if (r < 0)
        {
            qDebug("failed to get device descriptor");  //fail to get device descriptor
            return;
        }
        char buffer [1024] = {0};
        sprintf(buffer,"%04x:%04x (bus %d, device %d)\n",
               desc.idVendor, desc.idProduct,
               libusb_get_bus_number(dev), libusb_get_device_address(dev));
        QList<QVariant> list;
        list.push_back(desc.idVendor);
        list.push_back(desc.idProduct);
        USBList->addItem(buffer,QVariant(list));
    }
}

MainWindow::~MainWindow()
{
}
//打开usb设备
void MainWindow::OnClickOpenUSB()
{
    int r;
    ssize_t cnt;
    //libusb_context *ctx=nullptr; //context 上下文
    libusb_device **devs; //devices
    int i=0;
    QList<QVariant> current = USBList->currentData().toList();
    uint16_t idVendor= current[0].toUInt();
    uint16_t idProduct= current[1].toUInt();
    if("open"==openBtn->text())
    {
        cnt = libusb_get_device_list(nullptr, &devs); //获取设备列表
        if (cnt < 0)
        {
            qDebug("failed to get device list");  //fail to get device_list
            return;
        }

        //遍历查找你要的设备
        while ((dev = devs[i++]) != nullptr)
        {
            struct libusb_device_descriptor desc;  //设备信息描述符

            r= libusb_get_device_descriptor(dev, &desc); //获取设备信息描述符
            if (r < 0)
            {
                qDebug("failed to get device descriptor");  //fail to get device descriptor
                return;
            }
            if(desc.idVendor==idVendor && desc.idProduct==idProduct) //根据vid和pid查找某一具体设备
            {
                r=libusb_open(dev,&udev); //打开设备，获取设备操作符
                if(r<0)
                {
                    qDebug("failed to open device");  //fail to open device
                    qDebug("%s %d",libusb_error_name(r),r);
                    qDebug("udev: %d",udev);
                    return;
                }
                else
                {
                    qDebug("open device successfully");  //success to open device
                    qDebug("udev: %d",udev);
                }
                break;
            }
        }
        if(udev == nullptr)
        {
            qDebug("udev not found");
            return;
        }
        openBtn->setText("close");
        //libusb规定在用设备操作符发送数据前，需设置usb设备的配置模式和接口
        if ((r=libusb_set_configuration(udev, 1)) < 0)
        {
            //设置usb设备配置模式为1
            qDebug("error setting config #1: %s",  libusb_error_name(r));
            exit(1);
        }
        if ((r=libusb_claim_interface(udev, 0)) < 0)
        {
            //设置usb设备接口为0
            qDebug("error claiming interface #0:\n%s", libusb_error_name(r));
            exit(1);
        }
    }
    else
    {
        openBtn->setText("open");
        libusb_release_interface(udev, 0); //释放接口
        libusb_close(udev);  //关闭设备
        libusb_exit(ctx);  //退出libusb上下文

        udev=nullptr;

        qDebug("close usb");
    }
}


//发送指令
void MainWindow::OnClickSend()
{
    if(!udev)
    {
        return;
    }
    int r = 0;
    int outsize=0;


    unsigned char a[10] = {0x3A, 0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x0D};

    outsize=sizeof(a);

    r=libusb_bulk_transfer(udev,0x01,a,outsize,&outsize,5000);  //以块传输方式发送16进制数据 选择端口号为0x01
    //libusb_bulk_transfer函数参数解释：设备操作符 端口地址 要发送的数据 预计发送的字节数 实际发送的字节数 超时时间

    if (r < 0)
    {
        qDebug( "error writing:");
        qDebug("%s",libusb_error_name(r));
    }
    else
    {
        qDebug("%s %d %s","success: bulk write ",outsize," bytes");
        qDebug("3A 01 00 00 00 00 00 00 01 0D");
        this->read_usb();  //读取usb设备的反馈结果
    }
}

//读取usb设备的反馈结果
void MainWindow::read_usb()
{
    unsigned char tmps[50]={0};
    int i=0;
    int read_size_t=25;

    while(i<2)
    {
        int ret=0;
        ret = libusb_bulk_transfer(udev, 0x82, tmps, read_size_t, &read_size_t,5000);  //以块传输方式接收16进制数据 选择端口号为0x84
        //libusb_bulk_transfer函数参数解释：设备操作符 端口地址 要接受的数据 预计接受的字节数 实际接受的字节数 超时时间
        //libusb_bulk_transfer根据端口决定此时函数是接收还是发送数据

        if(ret >= 0)
        {
            qDebug("%s %d %s","success: bulk read ",read_size_t," bytes");
            QString strTmp;
            QString strOut;
            for(int i = 0; i < read_size_t; i++)
            {
                strTmp.sprintf("%02X ", tmps[i]);
                strOut.append(strTmp);
            }
            qDebug("%s",strOut.toStdString().c_str());
            qDebug("success");
            break;
        }
        else
        {
            qDebug("fail to read %d",ret);
            qDebug("error reading:");
            qDebug("%s",libusb_error_name(ret));
        }
        i++;
    }
    qDebug("finish2");
}
