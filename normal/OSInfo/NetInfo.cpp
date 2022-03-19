#include"NetInfo.h"


std::string GetMac(const char* ipaddr)										//根据ip获取mac
{
	unsigned char dwRouteMAC[6] = { 6 };
	ULONG uLen = 6;
	unsigned long dwGetWayIp = inet_addr(ipaddr);
	if (SendARP(dwGetWayIp, NULL, (PULONG)dwRouteMAC, &uLen) == NO_ERROR)
	{
		char dhost[1024] = { 0 };
		//当前网卡网关MAC
		memcpy(dhost, dwRouteMAC, 6);
		sprintf(dhost, "%02X:%02X:%02X:%02X:%02X:%02X",
			dwRouteMAC[0], dwRouteMAC[1], dwRouteMAC[2], dwRouteMAC[3], dwRouteMAC[4], dwRouteMAC[5]);
		return dhost;
	}
	return "";
}