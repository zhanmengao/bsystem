#ifndef QDESKTOPUTILS_H
#define QDESKTOPUTILS_H
#include<QFile>
#include<QString>
#include"../File/QPath/QPathHelper.hpp"
#include<QTextStream>
#include <sys/stat.h>
class QFileUtils
{
public:
    static int CreateDesktopLink(QString exec,QString appName,QString icon)
    {
        QString desktopFile = QPathHelper::GetUserFolder() + "Desktop/"+ appName +".desktop";
        QDir cheackDir = QDir(QPathHelper::GetUserFolder() + "Desktop/" );

        if( !cheackDir.exists() )
        {
            desktopFile = QPathHelper::GetUserFolder() + "/桌面/"+ appName +".desktop";
        }
        //创建桌面图标（快捷方式）
        QString sFilePath = desktopFile;
        QFile file(sFilePath);

        if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            return -1;
        }

        QTextStream out(&file);
        out<<"[Desktop Entry]"<<endl;
        out<<"Name="+ appName << endl;
        out<<"GenericName=Cloud Application"<<endl;
        out<<"Comment=VEsystem"<<endl;
        out<<"Exec="+ exec <<endl;
        out<<"Icon=" + icon <<endl;
        out<<"Terminal=false" <<endl;
        out<<"Type=Application" <<endl;
        out.flush();
        file.close();
        AddFileExeMode(sFilePath);
    }
    static int AddFileExeMode(  QString sFilePath   )
    {
        QByteArray tmpArray = sFilePath.toUtf8();
        const char* Xfilename = tmpArray.data();

        struct stat fileStat;
        stat( Xfilename ,&fileStat );
        chmod( Xfilename, fileStat.st_mode | S_IXUSR  );
        return 0;
    }
};

#endif // QDESKTOPUTILS_H
