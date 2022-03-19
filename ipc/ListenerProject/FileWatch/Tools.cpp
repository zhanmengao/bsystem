#include "Tools.h"
#include <windows.h>
#include <wchar.h>
#include <string>
#include <stringapiset.h>
using namespace std;

bool 	LoadConfig()
{
	ifstream ifs;
	ifs.open(GlobalDefine::configPath.c_str(), ios::in);
	if (!ifs.is_open())
	{
		cout << "Open config.ini error" << endl;
		return false;
	}
	char buf[1024];
	//配置文件
	if (ifs.getline(buf, sizeof(buf)))
	{
		string str(buf);
		//全部文件都监听
		GlobalDefine::allWatch = str == "1" ? true : false;;
	}
	memset(buf, 0, sizeof(buf));
	if (ifs.getline(buf, sizeof(buf)))
	{
		string str(buf);
		//全部文件都监听
		GlobalDefine::watchDOS = str == "1" ? true : false;;
	}
	memset(buf, 0, sizeof(buf));
	if (ifs.getline(buf, sizeof(buf)))
	{
		string str(buf);
		//全部文件都监听
		GlobalDefine::watchXLC = str == "1" ? true : false;;
	}
	memset(buf, 0, sizeof(buf));
	if (ifs.getline(buf, sizeof(buf)))
	{
		string str(buf);
		GlobalDefine::watchPDF = str == "1" ? true : false;;
	}
	memset(buf, 0, sizeof(buf));
	if (ifs.getline(buf, sizeof(buf)))
	{
		GlobalDefine::saveDataPath = (buf);
	}
	memset(buf, 0, sizeof(buf));
	if (ifs.getline(buf, sizeof(buf)))
	{
		GlobalDefine::fifoName = string2wstring(buf);
	}
	cout << "reload config sec!!" << endl;
	return true;
}

bool SaveDataToFile(const std::string& szFilePath, const std::string& text)
{
	ofstream ofs;
	ofs.open(szFilePath.c_str(), fstream::out || fstream::app);
	if (!ofs.is_open())
	{
		cout << "SaveDataToFile error" << endl;
		return false;
	}
	//写数据
	ofs << text << endl;
	ofs.close();
	printf("写文件成功，写入%d字节。\n", text.length());
	return true;
}


wstring string2wstring(const string& str)
{
	wstring result;
	//获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//多字节编码转换成宽字节编码  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //添加字符串结尾  
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}