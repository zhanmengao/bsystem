#pragma once
#ifdef __linux
#define _T(a) a
typedef char TCHAR;
#define _tcslen strlen
#define _tcsstr strstr
#endif
#ifdef _UNICODE
#define TTCHAR wchar_t
#else
#define TTCHAR char
#endif
#ifdef __cplusplus
#include<string>
#include<iostream>
#ifdef _UNICODE
#define stdstr std::wstring
#define isstream std::wistringstream
#define ReplaceAll ReplaceAllW
#define OUTSTREAM std::wostream
#define STDOUT std::wcout
#define TOSTR std::to_wstring
#else
#define stdstr std::string
#define isstream std::istringstream
#define ReplaceAll ReplaceAllA
#define OUTSTREAM std::ostream
#define STDOUT std::cout
#define TOSTR std::to_string
#endif
#endif
#include<string.h>
