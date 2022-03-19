
//written by wangbaoming1999@163.com
//20140323
//dll export

#ifndef _SOCEKETCLIET_H_
#define _SOCEKETCLIET_H_

#ifdef __cplusplus
extern "C" {
#endif

#define _ITCAST_OS_WIN32_  
#define _ITCAST_OS_LINUX_


#ifdef _ITCAST_OS_WIN32_ 	
#define ITCAST_FUNC_EXPORT(returnType)     __declspec(dllexport) returnType
#else
#ifdef _ITCAST_OS_LINUX
#define ITCAST_FUNC_EXPORT(returnType) __attribute__ ((visibility("default"))) returnType
#else	
#define ITCAST_FUNC_EXPORT(returnType)    returnType
#endif
#endif

#ifdef __cplusplus
}
#endif


#endif



