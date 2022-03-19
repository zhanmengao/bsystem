#include"ErrorInfo.h"

void main()
{
	TCHAR buf[1024] = { 0 };
	unsigned nSize = 1024;
	ErrorMessage(66, buf, nSize);
	printf("%s", Convert::UnicodeToASCII(buf).c_str());
}