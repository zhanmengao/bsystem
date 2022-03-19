#include <iostream>
#include <QtWidgets/QApplication>
#include "Watermark.h"
#include<QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList arguments = QApplication::arguments();
    arguments.removeAt(0);
    if(arguments.size()<=0)
    {
#ifdef Q_OS_WINDOWS
        char buffer[MAX_PATH] = {0};
        DWORD code = GetEnvironmentVariableA("USERNAME", buffer, MAX_PATH);
        arguments.push_back(buffer);

#else
        struct passwd *pwd = getpwuid(getuid());
        printf("login account：%s\n", pwd->pw_name);
        arguments.push_back(pwd->pw_name);
#endif
        arguments.push_back(SysInfo::GetHostName());
    }
    if(arguments.size()<10)
    {
        QMessageBox::information(NULL,"WaterMark","Parameter error,process exit");
        return 0;
    }
    int offset = -1;
    Watermark watermark;
    //date
    watermark.SetShowDate(arguments[++offset].toInt());
    watermark.SetShowMachineName(arguments[++offset].toInt());
    QString str;
    if(arguments[++offset].size()>0)
    {
        if(arguments[offset]!="0")
        {
            str+= "    ";
            str+=arguments[offset];
        }
    }
    if(arguments[++offset].size()>0)
    {
        if(arguments[offset]!="0")
        {
            str+= "    ";
            str+=arguments[offset];
        }
    }
    watermark.SetFontSize(arguments[++offset].toInt());
    watermark.SetAngle(arguments[++offset].toInt());
    int r = arguments[++offset].toInt();
    int g = arguments[++offset].toInt();
    int b = arguments[++offset].toInt();
    int alpha = arguments[++offset].toInt();
    watermark.SetColor(r,g,b,alpha);
    watermark.SetTextVar(1,str);
    watermark.show();
    return a.exec();
}


