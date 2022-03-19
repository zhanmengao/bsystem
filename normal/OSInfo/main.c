#include <stdio.h>
#include"NetInfo.h"
#include"CPUInfo.h"
#include"OSInfo.h"
#include"BIOSInfo.h"
#include"MemInfo.h"
int main() 
{
	struct NetInfo mNet = {0};
	GetNetInfo(&mNet);
	printf("IP %s	Mac %s \n", mNet.IP, mNet.Mac);

    struct CPUInfo mCPU = {0};
    GetCPUInfo(&mCPU);
    printf("producer %s name %s cpuMHZ %s cacheSize %s\n", mCPU.producer, mCPU.name,mCPU.cpuMHZ,mCPU.cacheSize);

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
}
