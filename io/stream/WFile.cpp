#include <Windows.h>
#include <iostream>
#include <tchar.h>
 
using namespace std;
 
int mainWfile()
{
	{
		//创建一个新文件
		HANDLE hfile = CreateFile(_T("first.cpp"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hfile == INVALID_HANDLE_VALUE)
		{
			cout << "创建文件失败" << endl;
		}
		else
		{
			cout << "first.cpp创建成功" << endl;
		}
		CloseHandle(hfile);
	}

	{
		// 以只写方式打开已存在的文件
		HANDLE hfile = CreateFile(_T("first.cpp"), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hfile)
		{
			cout << "open file error" << endl;
		}
		DWORD plocation_len;
		//设置文件指针(打开文件时文件指针默认处于文件开头位置,如果要往里面写数据则需要设置文件指针位置)
		plocation_len = SetFilePointer(hfile, 0, NULL, FILE_END);
		cout << "文件开头距文件指针字节数:" << plocation_len << endl;
		//向文件中写入数据
		char buffer[] = { "//hello my first create file!!!" };						//创建buffer里面存放要写的内容
		DWORD writelength;																		 //创建writelength,里面存放实际写入的长度,可以打印出来
		bool write_ret = WriteFile(hfile, &buffer, (DWORD)strlen(buffer), &writelength, NULL);
		if (write_ret == 0)
		{
			cout << " write file fault" << endl;
		}
		cout << "实际写入长度 " << writelength << endl;
		cout << "action over" << endl;

		//设置文件指针
		plocation_len = SetFilePointer(hfile, 0, NULL, FILE_END);
		cout << "文件开头距文件指针字节数:" << plocation_len << endl;

		//向文件中写入数据
		char buffer1[] = { "//ni hao zhong guo!" };
		DWORD writelength1;                                    //创建writelength,里面存放实际写入的长度,可以打印出来
		bool write_ret1 = WriteFile(hfile, &buffer1, (DWORD)strlen(buffer1), &writelength1, NULL);
		if (write_ret1 == 0)
		{
			cout << " write file fault" << endl;
		}
		cout << "实际写入长度 " << writelength1 << endl;
		cout << "action1 over" << endl;
		CloseHandle(hfile);
		//设置文件指针(将文件指针设置到文件头)
		//plocation_len = SetFilePointer(hfile,0,NULL,FILE_BEGIN);
		//cout << "文件开头距文件指针字节数:" << plocation_len << endl;
	}

   
	{
		HANDLE hFile = CreateFile(_T("first.cpp"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			cout << "open file error" << endl;
		}
		//获取文件大小
		int file_length1 = GetFileSize(hFile, NULL);
		cout << "文件大小为" << file_length1 << endl;
		//从文件中读数据
		char * pbuffer11 = (char*)malloc(file_length1 + 1);  //创建缓冲区存放读取的内容
		DWORD rel_read_len1 = 0;
		bool read_ret = ReadFile(hFile, pbuffer11, file_length1, &rel_read_len1, NULL);
		{
			if (read_ret == 0)
				cout << "read file error" << endl;
		}
		pbuffer11[file_length1] = '\0';
		cout << "读到文件全部内容:" << pbuffer11 << endl;
		cout << "实际读到的长度" << rel_read_len1 << endl;
		free(pbuffer11);
		//关闭文件流句柄
		CloseHandle(hFile);
		if (DeleteFile(_T("first.cpp")))
		{
			cout << "first.cpp被删除" << endl;
		}
	}

     
	return 0;
}