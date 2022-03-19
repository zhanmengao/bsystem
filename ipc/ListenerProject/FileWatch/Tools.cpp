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
	//�����ļ�
	if (ifs.getline(buf, sizeof(buf)))
	{
		string str(buf);
		//ȫ���ļ�������
		GlobalDefine::allWatch = str == "1" ? true : false;;
	}
	memset(buf, 0, sizeof(buf));
	if (ifs.getline(buf, sizeof(buf)))
	{
		string str(buf);
		//ȫ���ļ�������
		GlobalDefine::watchDOS = str == "1" ? true : false;;
	}
	memset(buf, 0, sizeof(buf));
	if (ifs.getline(buf, sizeof(buf)))
	{
		string str(buf);
		//ȫ���ļ�������
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
	//д����
	ofs << text << endl;
	ofs.close();
	printf("д�ļ��ɹ���д��%d�ֽڡ�\n", text.length());
	return true;
}


wstring string2wstring(const string& str)
{
	wstring result;
	//��ȡ��������С��������ռ䣬��������С���ַ�����  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //����ַ�����β  
	//ɾ��������������ֵ  
	result.append(buffer);
	delete[] buffer;
	return result;
}