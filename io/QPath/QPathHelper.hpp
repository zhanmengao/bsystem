#ifndef QPATHHELPER_HPP
#define QPATHHELPER_HPP
#include<QString>
#include<QCoreApplication>
#include<QDir>
#include<QStandardPaths>
class QPathHelper
{
public:
    static QString GetCurrentPath()
    {
        return QDir::currentPath() + "/";
    }
    static QString GetModuleFolder()
    {
        return QCoreApplication::applicationDirPath() + "/";
    }
    static QString GetModuleFilePath()
    {
        return qApp->applicationFilePath();
    }
    static QString GetUserFolder()
    {
        return QDir::homePath() + "/";
    }
    static QString GetRootFolder()
    {
        return QDir::rootPath();
    }
    static QString GetDesktop()
    {
        return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/";
    }
    static QString GetAppData()
    {
        return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/";
    }
    static QString GetTemp()
    {
        return QDir::tempPath() + "/";
    }
    static QString configPath()
    {
        QString dir;
#ifndef Q_OS_WIN
        // On Unix, use the AppConfigLocation for the settings, that's configurable with the XDG_CONFIG_HOME env variable.
        dir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

#else
        auto newLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

        // Check if this is the first time loading the new location
        if (!QFileInfo(newLocation).isDir())
        {
            // Migrate data to the new locations
            auto oldLocation = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

            // Only migrate if the old location exists.
            if (QFileInfo(oldLocation).isDir())
            {
                QDir().mkpath(newLocation);
            }
        }
        dir = newLocation;
#endif
        if (!dir.endsWith(QLatin1Char('/')))
            dir.append(QLatin1Char('/'));
        return dir;
    }
};

#endif // QPATHHELPER_HPP
