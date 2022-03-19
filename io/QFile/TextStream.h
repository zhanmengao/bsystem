#ifndef TEXTSTREAM_H
#define TEXTSTREAM_H
#include<QTextStream>
#include<QFile>
#include<QFileInfo>
#include<QDir>
void TextStreamMain_WriteOnly(QString filePath = "output.txt")
{
    QFile data(filePath);
    //打开文件：只写，清空文件内容
    if(data.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream out(&data);
        out<<"The magic number is :"<<49<<"\n";
    }
}

void TextStreamMain_ReadOnly(QString filePath = "D:/log.txt")
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        qDebug("line: %s",line.toStdString().c_str());
    }
}

void FileMain(QString filePath = "D:/logHook.txt")
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        qDebug("line: %s",line.constData());
    }
}

void ReadProcFile(QString filePath = "/proc/modules")
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream in(&file);
    QString line = in.readLine();
    while(!line.isNull())
    {
        qDebug("line: %s",line.toStdString().c_str());
        line = in.readLine();
    }
}

void PrintfFileInfo(QString filePath)
{
    QFileInfo info(filePath);
    if(!info.exists())
    {
        qDebug("PrintfFileInfo error");
        return;
    }

    //返回一个代码文件绝对路径的QDir对象。
    qDebug("absoluteDir: %s",info.absoluteDir().path().toStdString().data());

    //返回一个包含文件名的绝对路径
    qDebug("absoluteFilePath: %s",info.absoluteFilePath().toStdString().data());

    //返回文件的绝对路径，不包括文件名。
    qDebug("absolutePath: %s",info.absolutePath().toStdString().data());


    //只返回文件名，不包括路径。例如：
    qDebug("absoluteFilePath: %s",info.fileName().toStdString().data());

    //返回文件的名字，包括路径（可能是绝对路径，也可能是相对路径）
    qDebug("filePath: %s",info.filePath().toStdString().data());

    //返回文件的路径，不包括文件名。
    qDebug("path: %s",info.path().toStdString().data());
}
#endif // TEXTSTREAM_H
