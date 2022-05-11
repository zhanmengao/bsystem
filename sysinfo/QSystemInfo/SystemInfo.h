#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <QDir>
#include <QStandardPaths>
#include <QProcess>
#include<QHostInfo>
class SysInfo
{
public:
    static QString GetUsername()
    {
        QString userName;

        if (userName.isEmpty())
        {
            QStringList envList = QProcess::systemEnvironment();
            for (const QString& env : envList)
            {
                if (env.startsWith("USERNAME"))
                {
                    QStringList strList = env.split('=');
                    if (strList.size() >= 2)
                    {
                        userName = strList[1];
                    }
                }
            }
        }
        if (!userName.isEmpty())
        {
            return userName;
        }

#if defined(Q_OS_WIN)
        userName = QStandardPaths::displayName(QStandardPaths::HomeLocation);
#elif defined(Q_OS_LINUX)
        QProcess process;
        process.start("whoami", QStringList());
        process.waitForFinished();
        userName = QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed();
#endif
        return userName.isEmpty() ? QString("User") : userName;
    }
    static QString GetHostName()
    {
        return QHostInfo::localHostName();
    }
};


#endif // SYSTEMINFO_H
