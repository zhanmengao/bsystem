#include <QCoreApplication>
#include<QSettings>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //创建配置文件
    //参数1，代表新建ini文件的位置的绝对路径；  参数2，将设置存储在ini文件中
    QSettings setting(QCoreApplication::applicationDirPath()+"/test.ini", QSettings::IniFormat);

    //写入数据
    setting.setValue("/setting/value1", 1);
    setting.setValue("/setting/value2", 2);

    //切换到setting组之下
    setting.beginGroup("/setting");
    setting.setValue("value3", 3);

    //关闭切换分组
    setting.endGroup();

    //读取数据
    QStringList all = setting.childGroups();
    for(int i = 0;i<all.size();i++)
    {
        qDebug("%s",all[i].toStdString().c_str());
        setting.beginGroup(all[i]);
        QStringList child = setting.childKeys();
        for(int j = 0;j<child.size();j++)
        {
            qDebug("%s : %s",child[j].toStdString().c_str(),setting.value(child[j]).toString().toStdString().c_str());
        }
        setting.endGroup();
    }

    QSettings setting2("D:\\test2.ini", QSettings::IniFormat);
    all = setting2.childGroups();
    for(int i = 0;i<all.size();i++)
    {
        qDebug("%s",all[i].toStdString().c_str());
        setting2.beginGroup(all[i]);
        QStringList child = setting2.childKeys();
        for(int j = 0;j<child.size();j++)
        {
            qDebug("%s : %ls",child[j].toStdString().c_str(),setting2.value(child[j]).toString().toStdWString().c_str());
        }
        setting2.endGroup();
    }
    return a.exec();
}
