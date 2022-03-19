#include <QCoreApplication>
#include"QPathHelper.hpp"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug("%s ",QPathHelper::GetCurrentPath().toStdString().c_str());
    qDebug("%s ",QPathHelper::GetModuleFolder().toStdString().c_str());
    qDebug("%s ",QPathHelper::GetModuleFilePath().toStdString().c_str());
    qDebug("%s ",QPathHelper::GetUserFolder().toStdString().c_str());
    qDebug("%s ",QPathHelper::GetRootFolder().toStdString().c_str());
    qDebug("%s ",QPathHelper::GetDesktop().toStdString().c_str());
    qDebug("%s ",QPathHelper::GetAppData().toStdString().c_str());
    qDebug("%s ",QPathHelper::GetTemp().toStdString().c_str());
    qDebug("%s ",QPathHelper::configPath().toStdString().c_str());

    return a.exec();
}
