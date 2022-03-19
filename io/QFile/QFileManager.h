#ifndef QFILEMANAGER_H
#define QFILEMANAGER_H
#include<QStringList>
#include<QDir>
class QFileManager
{
public:

    static QStringList GetAllFileInfo(const char* nPath,const QStringList& nameFilters)
    {
        //设置要遍历的目录
        QDir dir(nPath);
        /*
        //设置文件过滤器
        QStringList nameFilters;
        //设置文件过滤格式
        nameFilters << "*.mp4";*/
        //将过滤后的文件名称存入到files列表中
        return dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    }
};

#endif // QFILEMANAGER_H
