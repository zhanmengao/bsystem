#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include<Windows.h>
#include<iostream>
using namespace std;
#define MAX_NETPACK_SIZE	4096
unsigned int CheckDataSize(char** szBuf, unsigned int size, unsigned int needSize)
{
	if (szBuf == nullptr)
	{
		return 0;
	}
	char* pData = *szBuf;
	//¿©»›
	if (needSize >= size)
	{
		unsigned int bufSize = ((needSize / MAX_NETPACK_SIZE) + 1) * MAX_NETPACK_SIZE;
		char* newBuffer = new char[bufSize];
		memset(newBuffer, 0, bufSize);
		memcpy(newBuffer, szBuf, size);
		delete[] pData;
		*szBuf = newBuffer;
		return bufSize;
	}
	//Àı–°
	else if (needSize < MAX_NETPACK_SIZE && size > MAX_NETPACK_SIZE)
	{
		char* newBuffer = new char[MAX_NETPACK_SIZE];
		memset(newBuffer, 0, MAX_NETPACK_SIZE);
		memcpy(newBuffer, szBuf, MAX_NETPACK_SIZE);
		delete[] szBuf;
		*szBuf = newBuffer;
		return MAX_NETPACK_SIZE;
	}
	return size;
}
void mainNew()
{
	char *buf = new char[MAX_NETPACK_SIZE*10];
	int maxSize = MAX_NETPACK_SIZE*10;
	while (1)
	{
		maxSize = CheckDataSize(&buf, maxSize, maxSize);
		cout << "Cur Size:" << maxSize << endl;
		Sleep(1000);
	}
}