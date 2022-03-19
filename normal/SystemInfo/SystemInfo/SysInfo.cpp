
#include <iostream>   
#include <string>  
#include <windows.h>    

#pragma warning(disable: 4996) // avoid GetVersionEx to be warned  

// ***** global macros ***** //  
static const int kMaxInfoBuffer = 256;
#define  GBYTES  1073741824    
#define  MBYTES  1048576    
#define  KBYTES  1024    
#define  DKBYTES 1024.0    

// ---- get os info ---- //  
void getOsInfo()
{
	// get os name according to version number  
	OSVERSIONINFO osver = { sizeof(OSVERSIONINFO) };
	GetVersionEx(&osver);
	std::string os_name;
	if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)
		os_name = "Windows 2000";
	else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
		os_name = "Windows XP";
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0)
		os_name = "Windows 2003";
	else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
		os_name = "windows vista";
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1)
		os_name = "windows 7";
	else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
		os_name = "windows 10";

	std::cout << "os name: " << os_name << std::endl;
	std::cout << "os version: " << osver.dwMajorVersion << '.' << osver.dwMinorVersion << std::endl;

}

// ---- get cpu info ---- //  
// save 4 register variables  
DWORD deax;
DWORD debx;
DWORD decx;
DWORD dedx;

// init cpu in assembly language  
void initCpu(DWORD veax)
{
	__asm
	{
		mov eax, veax
		cpuid
		mov deax, eax
		mov debx, ebx
		mov decx, ecx
		mov dedx, edx
	}
}

long getCpuFreq()
{
	int start, over;
	_asm
	{
		RDTSC
		mov start, eax
	}
	Sleep(50);
	_asm
	{
		RDTSC
		mov over, eax
	}
	return (over - start) / 50000;
}

std::string getManufactureID()
{
	char manuID[25];
	memset(manuID, 0, sizeof(manuID));

	initCpu(0);
	memcpy(manuID + 0, &debx, 4); // copy to array  
	memcpy(manuID + 4, &dedx, 4);
	memcpy(manuID + 8, &decx, 4);

	return manuID;
}

std::string getCpuType()
{
	const DWORD id = 0x80000002; // start 0x80000002 end to 0x80000004  
	char cpuType[49];
	memset(cpuType, 0, sizeof(cpuType));

	for (DWORD t = 0; t < 3; t++)
	{
		initCpu(id + t);

		memcpy(cpuType + 16 * t + 0, &deax, 4);
		memcpy(cpuType + 16 * t + 4, &debx, 4);
		memcpy(cpuType + 16 * t + 8, &decx, 4);
		memcpy(cpuType + 16 * t + 12, &dedx, 4);
	}

	return cpuType;
}

void getCpuInfo()
{
	std::cout << "CPU main frequency: " << getCpuFreq() << "MHz" << std::endl;
	std::cout << "CPU manufacture: " << getManufactureID() << std::endl;
	std::cout << "CPU type: " << getCpuType() << std::endl;
}

// ---- get memory info ---- //  
void getMemoryInfo()
{
	std::string memory_info;
	MEMORYSTATUSEX statusex;
	statusex.dwLength = sizeof(statusex);
	if (GlobalMemoryStatusEx(&statusex))
	{
		unsigned long long total = 0, remain_total = 0, avl = 0, remain_avl = 0;
		double decimal_total = 0, decimal_avl = 0;
		remain_total = statusex.ullTotalPhys % GBYTES;
		total = statusex.ullTotalPhys / GBYTES;
		avl = statusex.ullAvailPhys / GBYTES;
		remain_avl = statusex.ullAvailPhys % GBYTES;
		if (remain_total > 0)
			decimal_total = (remain_total / MBYTES) / DKBYTES;
		if (remain_avl > 0)
			decimal_avl = (remain_avl / MBYTES) / DKBYTES;

		decimal_total += (double)total;
		decimal_avl += (double)avl;
		char  buffer[kMaxInfoBuffer];
		sprintf_s(buffer, kMaxInfoBuffer, "total %.2f GB (%.2f GB available)", decimal_total, decimal_avl);
		memory_info.append(buffer);
	}
	std::cout << memory_info << std::endl;
}

int main(int argc, char *argv[])
{
	std::cout << "===os information===" << std::endl;
	getOsInfo();

	std::cout << "===cpu infomation===" << std::endl;
	getCpuInfo();

	std::cout << "===memory information===" << std::endl;
	getMemoryInfo();

	system("pause");
	return 0;
}
