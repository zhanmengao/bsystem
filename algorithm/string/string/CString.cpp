#include"CString.h"

#include"../../../../C++space/StringTools/StringTools/StrUtils.h"
void String_InitSize(struct cstring* pStr, size_t rSize, char rData)
{
	if (pStr == NULL)
	{
		return;
	}
	pStr->string = (char*)malloc((rSize + 1) * sizeof(char));
	pStr->nSize = rSize;
	memset(pStr->string, rData, rSize);
	pStr->string[rSize] = '\0';
}

void String_InitStr(struct cstring* pStr, const char* src, size_t srcSize)
{
	if (pStr == NULL || src == NULL)
	{
		return;
	}
	pStr->string = (char*)malloc((srcSize + 1) * sizeof(char));
	strcpy(pStr->string, src);
	pStr->nSize = srcSize;
}

void String_CopyFrom(struct cstring* dest, const struct cstring* const src)
{
	if (dest->string == NULL)
	{
		String_InitStr(dest, src->string, src->nSize);
		return;
	}
	if (src == dest || strcmp(dest->string, src->string) == 0)
	{
		return;
	}
	String_Free(dest);
	String_InitStr(dest, src->string, src->nSize);
}
void String_PushBack(struct cstring* pStr, const char* data, size_t nSize)
{
	String_Resize(pStr, nSize + pStr->nSize);
	strcat(pStr->string, data);
}
void String_Resize(struct cstring* pStr, size_t rSize)
{
	if (pStr == NULL)
	{
		return;
	}
	struct cstring str = { 0 };
	String_CopyFrom(&str, pStr);

	pStr->string = (char*)malloc((rSize + 1) * sizeof(char));
	pStr->nSize = rSize;
	memset(pStr->string, '\0', rSize);

	if (str.string)
	{
		strcpy(pStr->string, str.string);
	}
	String_Free(&str);
}
//将src的字符串指针移到dst
void String_Move(struct cstring *dst, struct cstring* src)
{
	if (src == NULL || src->string == NULL || dst == NULL)
	{
		return;
	}
	String_Free(dst);
	dst->string = src->string;
	dst->nSize = src->nSize;
	dst->string = NULL;
	dst->nSize = 0;
}
void String_Free(struct cstring* pStr)
{
	if (pStr == NULL || pStr->string == NULL)
	{
		return;
	}
	free(pStr->string);
	pStr->nSize = 0;
}
void String_ReplaceAllChar(struct cstring* pStr, char dest, char src)
{
	for (size_t i = 0; i < pStr->nSize; i++)
	{
		if (pStr->string[i] == dest)
		{
			pStr->string[i] = src;
		}
	}
}
void String_Find(struct cstring* pStr, const char* dest, const char *src)
{

}
int String_ReplaceAllStr(struct cstring* pStr, const char* src, const char *target)
{
	//计算差
	size_t ss = strlen(src);
	size_t ts = strlen(target);
	int need = (int)ts - (int)ss;										//计算每替换一个字符串，需要的内存，可能为负
	//统计遇到的目标字符串数量
	unsigned nCount = Count(pStr->string, pStr->nSize, src);			//匹配数量
	if (nCount <= 0)
	{
		return 1;
	}
	//计算新字符串所需内存
	unsigned needMemory = pStr->nSize + need*nCount + 1;
	if (needMemory > pStr->nSize)
	{
		String_Resize(pStr, needMemory);
	}
	//拷贝
	return replaceAll(pStr->string, pStr->nSize, src, target, &pStr->nSize);
}
void String_RemoveAllChar(struct cstring* pStr, char dest)
{
	struct cstring temp;
	String_InitSize(&temp, pStr->nSize, '\0');
	size_t tempJ = 0;
	for (size_t i = 0; i < pStr->nSize; i++)
	{
		if (pStr->string[i] != dest)
		{
			temp.string[tempJ++] = pStr->string[i];
		}
	}
	//交换指针
	char* pData = pStr->string;
	pStr->string = temp.string;
	pStr->nSize = tempJ;
	temp.string = pData;
	String_Free(&temp);
}
void String_UrlEncode(struct cstring* pStr)
{
	struct cstring strTemp = { 0 };
	size_t length = pStr->nSize;
	size_t bufferSize = 0;
	char buffer[1024] = { 0 };
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)pStr->string[i]) ||
			(pStr->string[i] == '-') ||
			(pStr->string[i] == '_') ||
			(pStr->string[i] == '.') ||
			(pStr->string[i] == '~'))
		{
			buffer[bufferSize++] = pStr->string[i];
			if (bufferSize >= sizeof(buffer))
			{
				//拼
				String_PushBack(&strTemp, buffer, bufferSize);
				memset(buffer, 0, sizeof(buffer));
				bufferSize = 0;
			}
		}
		else
		{
			//拼
			String_PushBack(&strTemp, buffer, bufferSize);
			memset(buffer, 0, sizeof(buffer));
			bufferSize = 0;
			if (pStr->string[i] == ' ')
			{
				buffer[bufferSize++] = '+';
			}
			else
			{
				buffer[bufferSize++] = '%';
				buffer[bufferSize++] = toHex((unsigned char)pStr->string[i] >> 4);
				buffer[bufferSize++] = toHex((unsigned char)pStr->string[i] % 16);
			}
		}
	}
	String_PushBack(&strTemp, buffer, bufferSize);
	String_CopyFrom(pStr, &strTemp);
	String_Free(&strTemp);
}
