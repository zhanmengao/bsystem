#include "stdafx.h"
#include "downloadFile.h"




vector<string> split(const string &s, const string &seperator)
{
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;


	/*
		http://192.168.50.35:8080/DataMapManager/veTerminal/veidp.exe
		vector<string> v = split(data_downloadAdd, "/");
	*/
	while (i != s.size())
	{
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0)
		{
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x])
				{
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0)
		{
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x])
				{
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j)
		{
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}

/***
* 取环境变量
*/
char* Get_ENV(char* hzName)
{

	char* hzName_v;
	size_t requiredSize;

	getenv_s(&requiredSize, NULL, 0, hzName);
	if (requiredSize == 0)
	{
		return NULL;
	}

	hzName_v = (char*)malloc(requiredSize * sizeof(wchar_t));
	if (!hzName_v)
	{
		return NULL;
	}
	getenv_s(&requiredSize, hzName_v, requiredSize, hzName);

	return hzName_v;

}


void KillProcessByPID(int pid)
{
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	BOOL bOkey = TerminateProcess(hProcess, 0);
	if (bOkey)
	{
		WaitForSingleObject(hProcess, 1000);
		CloseHandle(hProcess);
		hProcess = NULL;
	}
}


void TerminateProcessByName(const char* name)
{
	DWORD pid = 0;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	std::vector<DWORD> pids;

	if (Process32First(snapshot, &process))
	{
		do
		{
			std::string szExeFile = process.szExeFile;
			std::string szName = name;

			if (!strcmp(szExeFile.c_str(), szName.c_str()))
			{
				pid = process.th32ProcessID;
				pids.push_back(pid);
			}
		} while (Process32Next(snapshot, &process));
	}

	CloseHandle(snapshot);

	size_t size = pids.size();
	for (size_t i = 0; i < size; ++i)
	{
		KillProcessByPID(pids[i]);
	}

}

size_t writeFunc(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return fwrite(ptr, size, nmemb, stream);
}

int progressFunc(char *progress_data,
	double t, /* dltotal */
	double d, /* dlnow */
	double ultotal,
	double ulnow)
{
	printf("%s %g / %g (%g %%)\n", progress_data, d, t, d*100.0 / t);
	return 0;
}

string GetMd5(LPCSTR FileDirectory)
{
	HANDLE hFile = CreateFile(FileDirectory, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)																//如果CreateFile调用失败
	{
		cout << "CreateFile go wrong :" << GetLastError() << endl;									//提示CreateFile调用失败，并输出错误号。visual studio中可在“工具”>“错误查找”中利用错误号得到错误信息。
		CloseHandle(hFile);
		return "";
	}
	HCRYPTPROV hProv = NULL;
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == FALSE)       //获得CSP中一个密钥容器的句柄
	{
		cout << "CryptAcquireContext go wrong:" << GetLastError() << endl;
	}
	HCRYPTPROV hHash = NULL;
	if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash) == FALSE)									//初始化对数据流的hash，创建并返回一个与CSP的hash对象相关的句柄。这个句柄接下来将被CryptHashData调用。
	{
		cout << "CryptCreateHash go wrong:" << GetLastError() << endl;
	}
	DWORD dwFileSize = GetFileSize(hFile, 0);		//获取文件的大小
	if (dwFileSize == 0xFFFFFFFF)					//如果获取文件大小失败
	{
		cout << "GetFileSize go wrong:" << GetLastError() << endl;
	}

	BYTE* lpReadFileBuffer = new BYTE[dwFileSize];
	DWORD lpReadNumberOfBytes;
	if (ReadFile(hFile, lpReadFileBuffer, dwFileSize, &lpReadNumberOfBytes, NULL) == 0)			//读取文件
	{
		cout << "ReadFile go wrong:" << GetLastError() << endl;
	}
	if (CryptHashData(hHash, lpReadFileBuffer, lpReadNumberOfBytes, 0) == FALSE)				//hash文件
	{
		cout << "CryptHashData go wrong:" << GetLastError() << endl;
	}

	delete[] lpReadFileBuffer;
	CloseHandle(hFile);								//关闭文件句柄
	BYTE *pbHash;
	DWORD dwHashLen = sizeof(DWORD);

	if (CryptGetHashParam(hHash, HP_HASHVAL, NULL, &dwHashLen, 0))      //我也不知道为什么要先这样调用CryptGetHashParam，这块是参照的msdn       
	{
	}
	else
	{
		cout << "get length wrong" << endl;
	}
	if (pbHash = (BYTE*)malloc(dwHashLen))
	{
	}
	else
	{
		cout << "allocation failed" << endl;
	}

	string md5;

	char firstNum[32] = { 0 };

	if (CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0))            //获得md5值
	{
		for (DWORD i = 0; i < dwHashLen; i++)         //输出md5值
		{
			sprintf_s(firstNum, "%02x", pbHash[i]);
			md5 += firstNum;
			//printf("%02x", pbHash[i]);
		}

		cout << endl;
	}

	//	printf("%s", md5);

	//善后工作
	if (pbHash)
	{
		free(pbHash);
		pbHash = NULL;
	}
	if (CryptDestroyHash(hHash) == FALSE)          //销毁hash对象
	{
		cout << "CryptDestroyHash go wrong:" << GetLastError() << endl;
	}
	if (CryptReleaseContext(hProv, 0) == FALSE)
	{
		cout << "CryptReleaseContext go wrong:" << GetLastError() << endl;
	}
	return md5;
}




BOOL FileExistsStatus(const CHAR* path)
{
	DWORD dwAttribute = GetFileAttributes(path);
	if (dwAttribute == 0XFFFFFFFF)
	{
		return false; //0XFFFFFFFF表示文件不存在
	}
	else
	{
		return true;
	}
}

BOOL DownloadFiles(const CHAR* url, const CHAR* filename, string &hashCode)
{
	//将大写的都转换成小写
	transform(hashCode.begin(), hashCode.end(), hashCode.begin(), ::tolower);
	string md5 = "";
	//do{
	remove(filename);
	string s = filename;
	HRESULT hr = URLDownloadToFile(NULL, url, filename, 0, 0);
	if (hr == S_OK)
	{
		md5 = GetMd5(s.c_str());

		//将大写的都转换成小写
		transform(md5.begin(), md5.end(), md5.begin(), ::tolower);

		if (md5 == hashCode)
		{
			//MessageBox(0, hashCode.c_str(), "相等文件", 1);
			return TRUE;
		}
		else
		{
			remove(filename);
		}
	}
	else
	{
		remove(filename);
	}
	//} while (md5.c_str() != hashCode.c_str());
	//MessageBox(0, hashCode.c_str(), "文件", 1);
	return TRUE;
}

int HttpDownFile(const  string &strUrl, const  string &filename, string &hashCode)
{
	DownloadFiles(strUrl.c_str(), filename.c_str(), hashCode);
	return TRUE;
}


void Download(int instruct, bool ifSortSent, const string&  strRet)
{
	Json::Reader reader;
	Json::Value  resp;

	if (reader.parse(strRet, resp, false))
	{
		string data_downloadAdd = "";
		string data_hashCode = "";

		Json::Value  r_data = resp["data"];

		data_downloadAdd = r_data["filename"].asString();
		data_hashCode = r_data["md5"].asString();
		string type = r_data["updateType"].asString();

		if (data_downloadAdd.length() != 0)
		{
			//http://192.168.50.35:8080/DataMapManager/veTerminal/veidp.exe   截取exe名字：20180929135321_aaa.exe
			vector<string> v = split(data_downloadAdd, "/");
			string filename = v[v.size() - 1];

			char * strc = new char[strlen(filename.c_str()) + 1];
			strcpy(strc, filename.c_str());

			//从环境名取temp路径 C:\Users\week\AppData\Local\Temp\20180929135321_aaa.exe
			char buffer[128] = { 0 };
			char* temp = Get_ENV("temp");
			sprintf(buffer, "%s\\%s", temp, strc);
			if (temp)
			{
				free(temp);
				temp = NULL;
			}


			if (type == "1")	//1 强制更新
			{
				TerminateProcessByName(buffer);//终止进程
				if (HttpDownFile(data_downloadAdd, buffer, data_hashCode) == 1)
				{
					WinExec(buffer, SW_SHOW);
				}
			}
			else if (type == "2")//2 建议更新（弹窗 提示）
			{
				int ifupdate = MessageBox(0, TEXT("建议更新到最新版本"), TEXT("提示"), 1);
				if (ifupdate == 1)//点击确定 更新 如果进程在运行 终止
				{
					TerminateProcessByName(buffer);//终止进程
					if (HttpDownFile(data_downloadAdd, buffer, data_hashCode) == 1)
					{
						WinExec(buffer, SW_SHOW);
					}
				}
				else if (ifupdate == 2)//点击取消 返回
				{
					delete strc;
					return;
				}
			}
			delete strc;
		}
	}
}
