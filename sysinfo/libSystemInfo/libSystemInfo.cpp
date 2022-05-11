#include"libSystemInfo.h"
#include"../OSInfo/OSInfo.h"

UL libGetOSFullName(char* buffer, int& bufSize)
{
	std::string str = GetOSFullName();
	if (str.empty())
	{
		return GetLastError();
	}
	str = Convert::PChar2GolangBuf(str);
	strcpy(buffer, str.c_str());
	bufSize = str.size();
	return 0;
}