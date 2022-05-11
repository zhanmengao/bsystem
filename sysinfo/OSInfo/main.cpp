#include <stdio.h>
#include"NetInfo.h"
#include"BIOSInfo.h"
#include"OSInfo.h"
#include <stdio.h>
#include"CPUInfo.h"
#include"MemInfo.h"
#include<netdb.h>
int main() 
{
#ifdef _WIN32
	struct NetInfo mNet = {0};
	GetNetInfo(&mNet);
	auto mac = GetMac(mNet.IP);
	printf("IP %s	Mac %s  %s\n", mNet.IP, mNet.Mac,mac.c_str());
	vector<WMIBIOS> bios = GetBIOSInfo();
	printf("BIOS VERSION %ws \n", bios[0].SMBIOSBIOSVersion.c_str());

	printf("System Full Name %s \n", GetOSFullName().c_str());
    return 0;
#else
    struct NetInfo mNet = {0};
    GetNetInfo(&mNet);
    printf("IP %s	Mac %s \n", mNet.IP, mNet.Mac);

    struct CPUInfo mCPU = {0};
    GetCPUInfo(&mCPU);
    printf("producer %s name %s cpuMHZ %s cacheSize %s\n", mCPU.producer, mCPU.name,mCPU.cpuMHZ,mCPU.cacheSize);

    struct netent * n = getnetent();
    printf("netent %s n_addrtype %d \n",n->n_name,n->n_addrtype);

    struct protoent * proto = getprotoent();
    printf("p_name %s p_proto %d \n",proto->p_name,proto->p_proto);
    //getnameinfo()

    struct servent *ser = getservent();
    printf("serve %s port %d \n",ser->s_name,ser->s_port);

    char buffer[1024] = {0};
    GetHostName(buffer,sizeof(buffer));
    printf("Username %s Hostname %s \n",GetUser(),buffer);
    GetLinuxVersion(buffer);
    printf("LinuxVersion %s\n",buffer);

    struct BIOSInfoC biosInfo;
    GetBIOSInfo(&biosInfo);
    printf("BIOS ID %s\n",biosInfo.biosid);

    struct MemoryInfo memInfo;
    GetMemoryInfo(&memInfo);
    printf("MemTotal %s\n",memInfo.MemTotal);


    return 0;
#endif
}
