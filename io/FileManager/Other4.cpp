#include<Windows.h>
#include<iostream>
#include<vector>
#include<wchar.h>
using namespace std;
void main4()
{
	DWORD dwLen = GetLogicalDriveStrings(0, NULL);	//获取系统字符串长度.
	WCHAR * pszDriver = new WCHAR[dwLen];				//构建一个相应长度的数组.
	WCHAR * pStr = pszDriver;
	GetLogicalDriveStrings(dwLen, pszDriver);		//获取盘符字符串.
	vector<wstring> v;
	while (*pStr != '\0')
	{
		wcout << pStr << " ---- " << endl;
		v.push_back(pStr);
		pStr += lstrlen(pStr) + 1;			//定位到下一个字符串.加一是为了跳过'\0'字符串.
	}
	if (pszDriver != nullptr)
	{
		delete[] pszDriver;
		pszDriver = nullptr;
		pStr = nullptr;
	}
	int DType;
	int si = 0;
	BOOL fResult;
	unsigned _int64 i64FreeBytesToCaller;
	unsigned _int64 i64TotalBytes;
	unsigned _int64 i64FreeBytes;

	for (int i = 0; i < v.size(); ++i)
		//为了显示每个驱动器的状态，则通过循环输出实现，由于DStr内部保存的数据是A:\NULLB:\NULLC:\NULL，这样的信息，所以DSLength/4可以获得具体大循环范围
	{
		wcout << v[i].c_str() << ":";
		DType = GetDriveType(v[i].c_str());
		//GetDriveType函数，可以获取驱动器类型，参数为驱动器的根目录
		if (DType == DRIVE_FIXED)
		{
			cout << "硬盘";
		}
		else if (DType == DRIVE_CDROM)
		{
			cout << "光驱";
		}
		else if (DType == DRIVE_REMOVABLE)
		{
			cout << "可移动式磁盘";
		}
		else if (DType == DRIVE_REMOTE)
		{
			cout << "网络磁盘";
		}
		else if (DType == DRIVE_RAMDISK)
		{
			cout << "虚拟RAM磁盘";
		}
		else if (DType == DRIVE_UNKNOWN)
		{
			cout << "未知设备";
		}

		fResult = GetDiskFreeSpaceEx(v[i].c_str(),
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);
		//GetDiskFreeSpaceEx函数，可以获取驱动器磁盘的空间状态,函数返回的是个BOOL类型数据
		//通过返回的BOOL数据判断驱动器是否在工作状态
		if (fResult)
		{
			cout << " 总大小:" << (float)i64TotalBytes / 1024 / 1024 << " MB";//磁盘总容量
			cout << " 剩余大小:" << (float)i64FreeBytesToCaller / 1024 / 1024 << " MB";//磁盘剩余空间
		}
		else
		{
			cout << " 设备未准备好";
		}
		cout << endl;
		si += 4;
	}

}