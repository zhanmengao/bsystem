#ifndef _SYSTEMINFO_H 
#define _SYSTEMINFO_H


#define UL unsigned long
#define EXPORT extern "C" __declspec(dllexport)

EXPORT UL libGetOSFullName(char* buffer, int& bufSize);
#endif
