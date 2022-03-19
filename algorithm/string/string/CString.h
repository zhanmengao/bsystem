#ifndef _CSTRING_H
#define _CSTRING_H
#include<string.h>
#include<stdlib.h>

struct cstring
{
	char* string;
	size_t nSize;
};

void String_InitSize(struct cstring* pStr, size_t rSize, char rData);

void String_InitStr(struct cstring* pStr, const char* src, size_t srcSize);

void String_CopyFrom(struct cstring* dest, const struct cstring* const src);

void String_PushBack(struct cstring* dest, const char* data,size_t nSize);

void String_Resize(struct cstring* pStr, size_t rSize);

void String_Free(struct cstring* pStr);

void String_Move(struct cstring *dst,struct cstring* src);

void String_Find(struct cstring* pStr, const char* dest, const char *src);

void String_ReplaceAllChar(struct cstring* pStr, char dest, char src);

int String_ReplaceAllStr(struct cstring* pStr, const char* src, const char *target);

void String_RemoveAllChar(struct cstring* pStr, char dest);

void String_UrlEncode(struct cstring* pStr);
#endif