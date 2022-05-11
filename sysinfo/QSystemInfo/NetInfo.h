#ifndef NETINFO_H
#define NETINFO_H
#include<QString>
#include<QNetworkInterface>
class NetInfo
{
public:
    static QString GetHostIpAddress()
    {
        QString strIpAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        // 获取第一个本主机的IPv4地址
        int nListSize = ipAddressesList.size();
        for (int i = 0; i < nListSize; ++i)
        {
               if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                   ipAddressesList.at(i).toIPv4Address())
               {
                   strIpAddress = ipAddressesList.at(i).toString();
                   break;
               }
         }
         // 如果没有找到，则以本地IP地址为IP
         if (strIpAddress.isEmpty())
            strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
         return strIpAddress;
    }

    static QString GetHostMacAddress()
    {
        QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
        int nCnt = nets.count();
        QString strMacAddr = "";
        for(int i = 0; i < nCnt; i ++)
        {
            // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
            if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
            {
                strMacAddr = nets[i].hardwareAddress();
                break;
            }
        }
        return strMacAddr;
    }
};


#endif // NETINFO_H
