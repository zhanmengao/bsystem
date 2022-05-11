#include <QCoreApplication>
#include<QMap>
#include<QProcess>
#include<QDebug>
#include<sstream>
#include<string>
#include<vector>
typedef struct _usbdvinfo
{
    QString bindid;
    QString pidvid;
    QString dvname;
}LUSBDvInfo;
QList<LUSBDvInfo> lusbdvTables;
#define CMD_RESULT_BUF_SIZE 1024
int ExecuteCMD(const char *cmd, char *result)
{
    int iRet = -1;
    char buf_ps[CMD_RESULT_BUF_SIZE];
    char ps[CMD_RESULT_BUF_SIZE] = { 0 };
    FILE *ptr;

    strcpy(ps, cmd);

    if ((ptr = popen(ps, "r")) != NULL)
    {
        while (fgets(buf_ps, sizeof(buf_ps), ptr) != NULL)
        {
            strcat(result, buf_ps);
            if (strlen(result) > CMD_RESULT_BUF_SIZE)
            {
                break;
            }
        }
        pclose(ptr);
        ptr = NULL;
        iRet = 0;  // 处理成功
    }
    else
    {
        printf("popen %s error\n", ps);
        iRet = -1; // 处理失败
    }

    return iRet;
}
void Split(const std::string& s,std::vector<std::string>& sv,char flag)
{
    sv.clear();
    std::istringstream iss(s);
    std::string tmp;
    while(getline(iss,tmp,flag))
    {
        sv.push_back(tmp);
    }
}

std::string Replace(const std::string& s,const std::string& oldVal,const std::string& newVal)
{
    std::string ret(s);
    while(1)
    {
        std::size_t pos = 0;
        if((pos = ret.find(oldVal))!=std::string::npos)
        {
            ret.replace(pos,oldVal.length(),newVal);
        }
        else
        {
            break;
        }
    }
    return ret;
}

int QExecUSB()
{
    struct usbdvinfo
    {
        QString pidvid;
        QString dvname;
    };

    QMap<QString, QString> lusbdMap;

    QProcess *ps = new QProcess;
    ps->start("lsusb");
    ps->waitForFinished();
    QString strTemp = "";
    QStringList tmpList;
    strTemp = QString::fromLocal8Bit(ps->readAllStandardOutput());
    tmpList.clear();
    tmpList = strTemp.split("\n");

    foreach(QString str, tmpList)
    {
        QStringList hList = str.split(" ");
        if (hList.size() >= 7)
        {
            QString vidpid = hList.at(5);
            QString dvname = str.mid(str.indexOf(vidpid) + vidpid.length() + 1);
            usbdvinfo tdv;
            tdv.dvname = dvname;
            tdv.pidvid = vidpid;

            lusbdMap.insert(vidpid, dvname);
        }
    }

    lusbdMap.insert("5448:0004", "财政UKey20079");
    lusbdMap.insert("1483:c007", "招商银行UKey");
    lusbdMap.insert("8829:0010", "建设银行UKey");

    QProcess *ps2 = new QProcess;
    QStringList options2;
    options2 << "list" << "-l";
    ps2->start("usbip", options2);
    ps2->waitForFinished();

    if (ps2->exitCode() != 0)
    {
        qDebug("内核程序调用失败,无法正常启动内核中USB设备管理的相关进程");
        qDebug() << "exitCode: " << ps2->exitCode();
        qDebug() << "exitStatus: " << ps2->exitStatus();
        qDebug() << "errorString: " << ps2->errorString();
        qDebug() << "readAllStandardError: " << ps2->readAllStandardError();
    }


    QString strTemp2 = "";
    QStringList tmpList2;
    strTemp2 = QString::fromLocal8Bit(ps2->readAllStandardOutput());
    qDebug(strTemp2.toStdString().c_str());
    tmpList2.clear();
    tmpList2 = strTemp2.split("\n");

    lusbdvTables.clear();

    foreach(QString str2, tmpList2)
    {
        if (str2.indexOf("- busid") < 0)
        {
            continue;
        }
        QStringList bindlist = str2.split(" ");
        if (bindlist.size() >= 4)
        {
            QString bindid = bindlist.at(3);
            //qDebug() << bindid;
            QString pidvid = bindlist.at(4);
            pidvid.replace("(", "");
            pidvid.replace(")", "");
            //qDebug() << pidvid;
            //qDebug() << lusbdMap.take(pidvid);
            QString dvname = lusbdMap.take(pidvid);

            LUSBDvInfo lusbDvInfo;
            lusbDvInfo.pidvid = pidvid;
            lusbDvInfo.bindid = bindid;
            lusbDvInfo.dvname = dvname;

            lusbdvTables.append(lusbDvInfo);
        }
    }
    for(auto it = lusbdvTables.begin();it!=lusbdvTables.end();it++)
    {
        qDebug("pidvid = %s bindid = %s dvname = %s",it->pidvid.toStdString().c_str(),it->bindid.toStdString().c_str(),
               it->dvname.toStdString().c_str());
    }
    return 0;
}

std::vector<LUSBDvInfo> ExecUSB()
{
    std::vector<LUSBDvInfo> lusbdvTables;
    struct usbdvinfo
    {
        std::string pidvid;
        std::string  dvname;
    };
    char buffer[1024] = {0};
    ExecuteCMD("lsusb",buffer);
    printf("lsusb = \n%s \n",buffer);
    std::map<std::string, std::string> lusbdMap;
    std::vector<std::string> sv;
    Split(buffer,sv,'\n');
    foreach(std::string str, sv)
    {
        std::vector<std::string> vec;
        Split(str,vec,' ');
        if (vec.size() >= 7)
        {
            std::string vidpid = vec.at(5);
            std::string dvname = str.substr(str.find(vidpid) + vidpid.length() + 1);
            usbdvinfo tdv;
            tdv.dvname = dvname;
            tdv.pidvid = vidpid;

            lusbdMap.insert(std::make_pair(vidpid, dvname));
        }
    }


    memset(buffer,0,sizeof(buffer));
    ExecuteCMD("usbip list -l",buffer);
    printf("usbip = \n%s \n",buffer);
    Split(buffer,sv,'\n');
    foreach(std::string str2, sv)
    {
        if (str2.find("- busid") == std::string::npos)
        {
            continue;
        }
        std::vector<std::string> bindlist;
        Split(str2,bindlist,' ');
        if (bindlist.size() >= 4)
        {
            std::string bindid = bindlist.at(3);
            std::string pidvid = bindlist.at(4);
            pidvid = Replace(pidvid,"(", "");
            pidvid = Replace(pidvid,")", "");
            auto it = lusbdMap.find(pidvid);
            LUSBDvInfo lusbDvInfo;
            if(it!=lusbdMap.end())
            {
                lusbDvInfo.dvname = it->second.c_str();
            }
            lusbDvInfo.pidvid = pidvid.c_str();
            lusbDvInfo.bindid = bindid.c_str();


            lusbdvTables.push_back(lusbDvInfo);
        }
    }
    return lusbdvTables;
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QExecUSB();
    ExecUSB();

    return a.exec();
}
