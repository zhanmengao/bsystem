#include <QCoreApplication>
#include<QProcess>
#include"../../../../Network/RDP/UAMLinux/GetCloudAppOpenFile2020/Strategy.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    xFreerdp xf;
    xf.PORT = "3389";
    xf.RDS = "127.0.0.1";

    xf.appName = "excel";
    xf.exePath = "C:\\Program Files (x86)\\Microsoft Office\\Office15\\EXCEL.EXE";

    xf.PASSWORD = "*********";
    xf.USERNAME = "test_1234";

    xf.openMode = 0;
    xf.App_heigh = 1080;
    xf.App_width = 1920;

    xf.DOMAINNAME = "UAMCLUSTER";

    xf.SSPASSWORD = "*********";

    xf.appParameter = "";

    QProcess pro;
    pro.set

    return a.exec();
}
