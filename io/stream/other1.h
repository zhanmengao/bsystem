#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>
#include <io.h>
#include <sys\stat.h>


using namespace std;


int mainfilesize()
{
	char *filepath = "D:\\1.TXT";


	//方法一
	HANDLE handle = CreateFileA(filepath, FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (handle != INVALID_HANDLE_VALUE)
	{
		int size = GetFileSize(handle, NULL);
		cout << size << endl;
		CloseHandle(handle);
	}


	//方法二 C++流获取
	ifstream fin(filepath);
	if (fin.is_open())
	{
		fin.seekg(0, ios::end);
		int size = fin.tellg();
		fin.close();
		cout << size << endl;
	}


	//方法三 C标准获取
	FILE* file = fopen(filepath, "r");
	if (file)
	{
		int size = filelength(fileno(file));
		cout << size << endl;
		fclose(file);
	}


	//方法四
	struct _stat info;
	_stat(filepath, &info);
	int size = info.st_size;
	cout << size << endl;


	return 0;
}