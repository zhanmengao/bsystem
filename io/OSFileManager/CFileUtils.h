#ifndef _CFILEUTILS_H
#define _CFILEUTILS_H
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include<io.h>
#else
#endif

static FILE* FOpen(const char* filePath, const char* model)
{
	return fopen(filePath, model);
}

static size_t  FWrite(FILE* pFile, const char* buffer, size_t nDataSize)
{
	return fwrite(buffer, nDataSize, 1, pFile);
}
static size_t  FPuts(FILE* pFile, const char* str)
{
	return fputs(str, pFile);
}
static size_t  FRead(FILE* pFile, char* buffer, size_t nDataSize)
{
	return fread(buffer, nDataSize, 1, pFile);
}
static int  FGets(FILE* pFile, char* buffer,size_t nBufferSize)
{
	return fgets(buffer, nBufferSize, pFile) != NULL;
}
static int Getfd(FILE* pFile)
{
	return fileno(pFile);
}

static long FSize(FILE* pFile)
{
#ifdef _WIN32
	return filelength(Getfd(pFile));
#else
	long pos = ftell(pFile);
	fseek(pFile,0,SEEK_END);
	long nSize = ftell(pFile);
	fseek(pFile,pos,SEEK_SET);
	return nSize;
#endif
}



static int FEnd(FILE* pFile)
{
	return feof(pFile);
}


//SEEK_SET  0 SEEK_CUR  1 SEEK_END  2
static int FSeek(FILE* pFile,long offset,int way)
{
	return fseek(pFile, offset, way);
}
static void FClose(FILE** ppFile)
{
	if (ppFile == NULL || *ppFile == NULL)
	{
		return;
	}
	fclose(*ppFile);
	*ppFile = NULL;
}
#endif



/*
r	以只读方式打开文件，该文件必须存在
r+	以读/写方式打开文件，该文件必须存在
rb+	以读/写方式打开一个二进制文件，只允许写数�?
rt+	以读/写方式打开一个文本文件，允许读和�?
w	打开只写文件，若文件存在则文件长度清为零，即该文件内容会消失；若文件不存在则创建该文�?
w+	打开可读/写文件，若文件存在则文件长度清为零，即该文件内容会消失；若文件不存在则创建该文件
a	以附加的方式打开只写文件。若文件不存在，则会创建该文件；如果文件存在，则写入的数据会被加到文件尾后，
即文件原先的内容会被保留（EOF 符保留）
a+	以附加方式打开可读/写的文件。若文件不存在，则会创建该文件，如果文件存在，则写入的数据会被加到文件尾后，
即文件原先的内容会被保留（EOF符不保留�?
wb	以只写方式打开或新建一个二进制文件，只允许写数�?
wb+	以读/写方式打开或新建一个二进制文件，允许读和写
wt+	以读/写方式打开或新建一个文本文件，允许读和�?
at+	以读/写方式打开一个文本文件，允许读或在文本末追加数据
ab+	以读/写方式打开一个二进制文件，允许读或在文件末追加数�?
*/