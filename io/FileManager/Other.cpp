#include"FileManager.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <io.h> 
#include <time.h> 
#include <direct.h> 
#include <conio.h> 
#include <ctype.h> 
void getFiles(string path, vector<string>& files)
{
	// 文件句柄
	long hFile = 0;
	// 文件信息
	struct _finddata_t fileinfo;

	string p;

	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			// 保存文件的全路径
			files.push_back(p.assign(path).append("\\").append(fileinfo.name));

		} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
		_findclose(hFile);
	}
}

//2.获取指定目录下的所有文件（搜索子文件夹）
void getAllFiles(string path, vector<string>& files)
{
	//文件句柄
	long hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{  //比较文件类型是否是文件夹
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					//递归搜索
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
		_findclose(hFile);
	}
}


/*
path: 指定目录
files: 保存结果
fileType: 指定的文件格式，如 .jpg
*/
void getAllFiles(string path, vector<string>& files, string fileType)
{
	// 文件句柄
	long hFile = 0;
	// 文件信息
	struct _finddata_t fileinfo;

	string p;

	if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileinfo)) != -1)
	{
		do
		{
			// 保存文件的全路径
			files.push_back(p.assign(path).append("\\").append(fileinfo.name));

		} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1

		_findclose(hFile);
	}
}

int main1(int argc, char** argv)
{
	vector<string> temp;
	getFiles(TEST_ROOT, temp);
	for (int i = 0; i < temp.size(); ++i)
	{
		cout << temp[i] << endl;
	}
	system("pause");
	return 0;
}

/* The use of the 32-bit _find functions to print a list
of all files (and their attributes) in the current directory. */
/* Don't forget to put the .h extension to the header files */

#define SIZE 50 

int mainFile()
{
	// char   path[50] = "C://WINNT//System32//config"; - Windows 2000 
	char   path[50] = "C://Windows//System32//config";
	struct _finddata_t c_file;
	intptr_t   hFile;
	char buf[SIZE];

	printf("Change to %s/n", path);
	if (_chdir(path))
	{
		printf("Unable to locate the directory: %s/n", path);
		exit(1);
	}
	else
		/* Find first in the current directory */
		hFile = _findfirst("*.*", &c_file);

	/* List the files... */
	printf("Listing of files in the directory %s/n/n", path);
	printf("/nRDO HID SYS ARC  FILE                            DATE %20c SIZE/n", ' ');
	printf("--- --- --- ---  ----                             ---- %20c ----/n", ' ');
	printf((c_file.attrib & _A_RDONLY) ? " Y  " : " N  ");
	printf((c_file.attrib & _A_SYSTEM) ? " Y  " : " N  ");
	printf((c_file.attrib & _A_HIDDEN) ? " Y  " : " N  ");
	printf((c_file.attrib & _A_ARCH) ? " Y  " : " N  ");
	// unsafe version is ctime() 
	ctime_s(buf, SIZE, &(c_file.time_write));
	printf(" %-30s %.20s  %9ld/n", c_file.name, buf, c_file.size);

	/* Find the rest of the files */
	while (_findnext(hFile, &c_file) == 0)
	{
		printf((c_file.attrib & _A_RDONLY) ? " Y  " : " N  ");
		printf((c_file.attrib & _A_SYSTEM) ? " Y  " : " N  ");
		printf((c_file.attrib & _A_HIDDEN) ? " Y  " : " N  ");
		printf((c_file.attrib & _A_ARCH) ? " Y  " : " N  ");
		// unsafe version is ctime() 
		ctime_s(buf, SIZE, &(c_file.time_write));
		printf(" %-30s %.20s  %9ld/n", c_file.name, buf, c_file.size);
	}
	_findclose(hFile);

	return 0;
}