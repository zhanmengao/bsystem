#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "infoCollect.h"
#include <windows.h>
//base 屏幕截图 转换 base64
#include "../../../../Network/SocketEncode/SocketEncode/base64.h"
#include"../../SystemInfo/SystemInfo/SysInfoUtils.h"
#include<Gdiplus.h>
#include<direct.h>
#include<exception>
#include"veidp.h"
#include"../../../Process/WindowsProcessHelper/CreateProcess/ProcessHelper.h"
#include"../../../Process/WindowsProcessHelper/Fork/ForkUtils.h"
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#include"../../../File/FileManager/FileManager/FileManager.h"
#include"../../../Setting/Readini/INIManager.hpp"
#include"GlobalData.h"
#include"IDPEvent.h"
#include"../../../Setting/WDesktop/LnkUtils.h"
#include"../../../Error/WErrorUtils/ErrorInfo.h"

static tm * Globalltm = 0;

Json::Value instructGlobal;		//指令 1 保存的每条指令数据
bool instructGlobalIF = TRUE;	//指令 1 判断时候不需要在重复发送数据


int coutn = 0;//计算全局发送一次


string GetApplicationDir()
{
	char file_path[260];
	GetModuleFileNameA(NULL, file_path, sizeof(file_path));
	PathRemoveFileSpecA(file_path);
	return file_path;
}

ULONG GetHostOrder(const string strIp)
{
	ULONG iIp = inet_addr(strIp.c_str());
	iIp = ntohl(iIp);
	return iIp;
}


Json::Value GetTCPportlist()
{
	int iErrno;
	PMIB_TCPTABLE_OWNER_PID pMibTcpTableOwnerPid;
	DWORD dwSize = 0;
	//TCHAR szBuffer[1024];
	int i;

	//https://docs.microsoft.com/zh-cn/windows/win32/api/iphlpapi/nf-iphlpapi-getextendedtcptable?redirectedfrom=MSDN

	if ((iErrno = GetExtendedTcpTable(NULL, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) != NO_ERROR)
	{
		if (iErrno != ERROR_INSUFFICIENT_BUFFER)
		{
			printf("GetExtendedTcpTable Error: %d\n", iErrno);
			return FALSE;
		}
	}
	pMibTcpTableOwnerPid = (PMIB_TCPTABLE_OWNER_PID)malloc(dwSize);
	if (pMibTcpTableOwnerPid == NULL)
	{
		return FALSE;
	}
	if ((iErrno = GetExtendedTcpTable(pMibTcpTableOwnerPid, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) != NO_ERROR)
	{
		printf("GetExtendedTcpTable Error: %d\n", iErrno);
		return FALSE;
	}
	Json::Value TcpSoftList;
	for (i = 0; i < (int)pMibTcpTableOwnerPid->dwNumEntries; i++)
	{

		IN_ADDR localAddr;
		IN_ADDR remoteAddr;
		CHAR szLocalAddr[24];
		CHAR szRemoteAddr[24];
		USHORT usRemotePort;
		CHAR szState[24];

		localAddr.S_un.S_addr = pMibTcpTableOwnerPid->table[i].dwLocalAddr;
		remoteAddr.S_un.S_addr = pMibTcpTableOwnerPid->table[i].dwRemoteAddr;

		sprintf(szLocalAddr, "%s", inet_ntoa(localAddr));
		sprintf(szRemoteAddr, "%s", inet_ntoa(remoteAddr));

		switch (pMibTcpTableOwnerPid->table[i].dwState)
		{
		case MIB_TCP_STATE_CLOSED:
			sprintf(szState, "%s", "CLOSED");
			break;
		case MIB_TCP_STATE_LISTEN:
			sprintf(szState, "%s", "LISTENING");
			break;
		case MIB_TCP_STATE_SYN_SENT:
			sprintf(szState, "%s", "SYN_SENT");
			break;
		case MIB_TCP_STATE_SYN_RCVD:
			sprintf(szState, "%s", "SYN_RCVD");
			break;
		case MIB_TCP_STATE_ESTAB:
			sprintf(szState, "%s", "ESTABLISHED");
			break;
		case MIB_TCP_STATE_FIN_WAIT1:
			sprintf(szState, "%s", "FIN_WAIT_1");
			break;
		case MIB_TCP_STATE_FIN_WAIT2:
			sprintf(szState, "%s", "FIN_WAIT_2");
			break;
		case MIB_TCP_STATE_CLOSE_WAIT:
			sprintf(szState, "%s", "CLOSE_WAIT");
			break;
		case MIB_TCP_STATE_CLOSING:
			sprintf(szState, "%s", "CLOSING");
			break;
		case MIB_TCP_STATE_LAST_ACK:
			sprintf(szState, "%s", "LAST_ACK");
			break;
		case MIB_TCP_STATE_TIME_WAIT:
			sprintf(szState, "%s", "TIME_WAIT");
			break;
		case MIB_TCP_STATE_DELETE_TCB:
			sprintf(szState, "%s", "DELETE_TCB");
			break;
		}

		usRemotePort = strcmp(szState, "LISTENING") == 0 ? 0 : ntohs((USHORT)pMibTcpTableOwnerPid->table[i].dwRemotePort);

		Json::Value tcp;
		char usLocalPortSendBuf[32] = { '\0' };
		char usRemotePortSendBuf[32] = { '\0' };
		char usPidSendBuf[6] = { '\0' };

		sprintf(usLocalPortSendBuf, "%s:%d", szLocalAddr, ntohs((USHORT)pMibTcpTableOwnerPid->table[i].dwLocalPort));
		sprintf(usRemotePortSendBuf, "%s:%d", szRemoteAddr, usRemotePort);
		sprintf(usPidSendBuf, "%d", pMibTcpTableOwnerPid->table[i].dwOwningPid);

		tcp["Local Address"] = usLocalPortSendBuf;
		//tcp["Foreign Address"] = usRemotePortSendBuf;
		tcp["State"] = szState;
		tcp["PID"] = usPidSendBuf;

		TcpSoftList.append(tcp);
		//printf("%s\t%s\t%s\t%s\n", usLocalPortSendBuf, usRemotePortSendBuf, usPidSendBuf, szState);

	}

	free(pMibTcpTableOwnerPid);
	return TcpSoftList;
}

Json::Value GetUDPportlist()
{
	int iErrno;
	PMIB_UDPTABLE_OWNER_PID pMibUdpTableOwnerPid;
	DWORD dwSize = 0;
	int i;

	//https://docs.microsoft.com/zh-cn/windows/win32/api/iphlpapi/nf-iphlpapi-getextendedudptable?redirectedfrom=MSDN


	if ((iErrno = GetExtendedUdpTable(NULL, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0)) != NO_ERROR)
	{
		if (iErrno != ERROR_INSUFFICIENT_BUFFER)
		{
			printf("GetExtendedUdpTable Error: %d\n", iErrno);
			return FALSE;
		}
	}
	pMibUdpTableOwnerPid = (PMIB_UDPTABLE_OWNER_PID)malloc(dwSize);
	if (pMibUdpTableOwnerPid == NULL)
	{
		return FALSE;
	}
	if ((iErrno = GetExtendedUdpTable(pMibUdpTableOwnerPid, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0)) != NO_ERROR)
	{
		printf("GetExtendedUdpTable Error: %d\n", iErrno);
		return FALSE;
	}
	Json::Value UdpSoftList;
	for (i = 0; i < (int)pMibUdpTableOwnerPid->dwNumEntries; i++)
	{
		IN_ADDR localAddr;
		CHAR szLocalAddr[24];

		localAddr.S_un.S_addr = pMibUdpTableOwnerPid->table[i].dwLocalAddr;
		sprintf(szLocalAddr, "%s", inet_ntoa(localAddr));
		Json::Value udp;
		char usLocalPortSendBuf[32] = { '\0' };
		char usPidSendBuf[6] = { '\0' };

		sprintf(usLocalPortSendBuf, "%s:%d", szLocalAddr, ntohs((USHORT)pMibUdpTableOwnerPid->table[i].dwLocalPort));
		sprintf(usPidSendBuf, "%d", pMibUdpTableOwnerPid->table[i].dwOwningPid);


		udp["Local Address"] = usLocalPortSendBuf;
		//udp["Foreign Address"] = "*:*";
		udp["State"] = "*";
		udp["PID"] = usPidSendBuf;
		UdpSoftList.append(udp);
		//printf("%s\t%s\n", usLocalPortSendBuf, usPidSendBuf);

	}

	free(pMibUdpTableOwnerPid);
	return UdpSoftList;
}


string wbLoginName()
{
	char buffer[MAX_PATH] = { 0 };
	DWORD code = GetEnvironmentVariable(TEXT("USERNAME"), buffer, MAX_PATH);
	return buffer;
}

string region()
{
	DWORD dwVersion = 0;
	WKSTA_INFO_100 *wkstaInfo = NULL;
	NET_API_STATUS netStatus = NetWkstaGetInfo(NULL, 100, (BYTE **)&wkstaInfo);
	if (netStatus == NERR_Success)
	{
		DWORD dwMajVer = wkstaInfo->wki100_ver_major;
		DWORD dwMinVer = wkstaInfo->wki100_ver_minor;
		dwVersion = (DWORD)MAKELONG(dwMinVer, dwMajVer);
		NetApiBufferFree(wkstaInfo);
	}

	std::string s1 = Convert::UnicodeToUtf8(wkstaInfo->wki100_langroup);
	return s1;
}


string wbUptime()
{
	char* str1 = NULL;
	char str2[1024] = { 0 };

	DWORD iRunTime = GetTickCount();
	const int Num1 = 1000;
	const int Num2 = 1900;
	time_t nowTime;
	time(&nowTime);
	time_t systemUpTime = nowTime - (iRunTime / Num1);
	sprintf(str2, "%ld", systemUpTime);

	return str2;
}


string wbHostName()
{
	char buffer[MAX_PATH] = { 0 };
	DWORD code = GetEnvironmentVariable(TEXT("USERDOMAIN"), buffer, MAX_PATH);
	return buffer;
}

string wbOsInfo(char* str)
{
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 0;
	TCHAR data[MAX_PATH] = { 0 };
	DWORD data11 = NULL;
	int ret;
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return "None";
	}

	DWORD dwValueCount = 0, maxValueNameLen = 0, maxValueDataLen = 0;
	ret = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwValueCount, &maxValueNameLen, &maxValueDataLen, NULL, NULL);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return "None";
	}

	dwSize = maxValueDataLen;
	ret = RegQueryValueEx(hKey, str, NULL, &dwType, (LPBYTE)data, &dwSize);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return "None";
	}
	string  strWinVerName = data;
	RegCloseKey(hKey);
	return data;
}

string wbOsInfo11(char* str)
{
	HKEY hKey;
	DWORD dwType = REG_DWORD;
	DWORD dwSize = 0;
	//TCHAR data[MAX_PATH] = { 0 };
	DWORD data11 = NULL;
	int ret;

	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0,
		KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return "None";
	}
	DWORD dwValueCount = 0, maxValueNameLen = 0, maxValueDataLen = 0;
	ret = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwValueCount,
		&maxValueNameLen, &maxValueDataLen, NULL, NULL);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return "None";
	}
	dwSize = maxValueDataLen;
	ret = RegQueryValueEx(hKey, str, NULL, &dwType, (BYTE*)&data11, &dwSize);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return "None";
	}
	char str1[16] = { 0 };
	itoa(data11, str1, 10);

	string strWinVerName(str1);
	RegCloseKey(hKey);
	return strWinVerName;
}


string wbOsCpuInfo(char* str)
{
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 0;
	TCHAR data[MAX_PATH] = { 0 };
	DWORD data11 = NULL;
	int ret;

	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0,
		KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return "None";
	}
	DWORD dwValueCount = 0, maxValueNameLen = 0, maxValueDataLen = 0;
	ret = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwValueCount,
		&maxValueNameLen, &maxValueDataLen, NULL, NULL);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return "None";
	}
	dwSize = maxValueDataLen;
	ret = RegQueryValueEx(hKey, str, NULL, &dwType, (LPBYTE)data, &dwSize);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return "None";
	}
	RegCloseKey(hKey);
	return data;
}




Json::Value runProcess()
{
	LPCWSTR pwszProcName = NULL;
	HANDLE hProcess = NULL;
	HANDLE hProcSnap = INVALID_HANDLE_VALUE;
	PROCESSENTRY32W pe32w = { 0 };
	Json::Value RetPro;
	hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcSnap == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	ZeroMemory(&pe32w, sizeof(PROCESSENTRY32W));
	pe32w.dwSize = sizeof(PROCESSENTRY32W);
	if (!Process32FirstW(hProcSnap, &pe32w))
	{
		return 0;
	}

	Json::Value PidPro;
	string str1;
	do
	{
		str1.clear();
		char str2[1024] = { 0 };
		sprintf(str2, "%u", pe32w.th32ProcessID);

		char str3[1024] = { 0 };
		sprintf(str3, "%ws", pe32w.szExeFile);

		PidPro["Pid"] = str2;
		PidPro["Pro"] = str3;
		RetPro.append(PidPro);

	} while (Process32NextW(hProcSnap, &pe32w));
	return RetPro;
}


// send message
void StartSendMessage(CProducer *producer, Json::Value& MQmessage, string Tag, const string& ip)
{
	try
	{
		auto ifconfig = SysInfoUtils::GetNetInfo();
		CSendResult result;

		CMessage *msg = CreateMessage(IDPManager::Config.Topic.c_str());
		SetMessageTags(msg, Tag.c_str());
		SetMessageKeys(msg, ifconfig.strMac.c_str());

		SetMessageBody(msg, MQmessage.toStyledString().c_str());
		SendMessageSync(producer, msg, &result);
		DestroyMessage(msg);
		if (result.sendStatus == CSendStatus::E_SEND_OK)
		{
			LOG_DAILY(_T("SendMessageSync Sec    group:" + IDPManager::Config.Group + "    ip" + ip +
				"    Tag : " + Tag + "    Topic " + IDPManager::Config.Topic));
		}
		else
		{
			LOG(_T("SendMessageSync Error    ip" + ip +
				"    Tag : " + Tag + "    Topic " + IDPManager::Config.Topic +
				"Error:" + to_string(result.sendStatus)));
		}
	}
	catch (exception & e)
	{
		LOG(_T("SendMessageSync Error    ip" + ip +
			"    Tag : " + Tag + "    Topic " + IDPManager::Config.Topic));
	}
}

void RocketMQLink(Json::Value& MQmessage, const char* MQtag)
{
	if (producer != NULL)
	{
		ShutdownProducer(producer);
		DestroyProducer(producer);
	}
	producer = CreateProducer(IDPManager::Config.Group.c_str());
	SetProducerNameServerAddress(producer, IDPManager::Config.IP.c_str());
	string str = MQmessage.toStyledString();
	SetProducerMaxMessageSize(producer, str.size()*1.2f);
	StartProducer(producer);
	StartSendMessage(producer, MQmessage, MQtag, IDPManager::Config.IP);
}


int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;
	UINT  size = 0;
	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return -1;
}


void PrintLog(CString logText)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);
	CString Tt;
	Tt.Format("%d:%d:%d  %s", ltm->tm_hour, ltm->tm_min, ltm->tm_sec, logText);
}



BOOL SetImaeTextDate(string filename1)
{

	time_t now = time(0);
	tm *ltm = localtime(&now);
	Globalltm = ltm;
	/**
	* 在图片上加文字
	* ============================================================================================== */

	//https://docs.microsoft.com/zh-cn/windows/win32/api/gdiplusheaders/nl-gdiplusheaders-image
	//https://docs.microsoft.com/zh-cn/windows/win32/api/gdiplusgraphics/nl-gdiplusgraphics-graphics
	//

	stdstr filename0;
	filename0 = filename1.c_str();

	const WCHAR* filename = Convert::ASCIIToWideByte(filename0).c_str();

	Gdiplus::Image image2(filename);
	Gdiplus::Graphics imageGraphics(&image2);
	imageGraphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

	CString wxText;
	char buffer2[MAX_PATH] = { 0 };
	DWORD code = GetEnvironmentVariable("OS", buffer2, MAX_PATH);
	//sprintf(buffer2, "%s\\Screen.jpg", buffer1);



	wxText.Format(_TEXT("%d 年 %.2d 月 %.2d 日 %.2d:%.2d:%.2d\n%s"),
		ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec, buffer2);

	Gdiplus::FontFamily fontFamily(L"Microsoft YaHei");
	Gdiplus::Font myFont(&fontFamily, 20, Gdiplus::FontStyleRegular, Gdiplus::UnitPoint);
	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));
	Gdiplus::PointF school_site((Gdiplus::REAL)10, (Gdiplus::REAL)10);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentNear);

	auto wx = wxText.GetBuffer();
	const WCHAR* wxText1 = Convert::ASCIIToWideByte(string(wx)).c_str();
	wxText.ReleaseBuffer();

	imageGraphics.DrawString(wxText1, wcslen(wxText1), &myFont, school_site, &format, &blackBrush);

	CString newfilename = filename;
	newfilename.Replace(".jpg", "W.jpg");
	CLSID pngClsid;
	GetEncoderClsid(L"image/jpeg", &pngClsid);

	wx = newfilename.GetBuffer();
	const WCHAR* filename111 = Convert::ASCIIToWideByte(string(wx)).c_str();
	newfilename.ReleaseBuffer();

	if (image2.Save(filename111, &pngClsid, NULL) == Gdiplus::Ok)
	{
		PrintLog(filename111);
		return TRUE;
	}
	else
	{
		PrintLog(L"SetImaeTextDate Error");
		return FALSE;
	}
	return 0;
}

int GetWindowScreen2()
{
	time_t now = time(0);
	tm *ltm = localtime(&now);
	CString filename;

	char buffer1[MAX_PATH] = { 0 };
	DWORD code = GetEnvironmentVariable("USERPROFILE", buffer1, MAX_PATH);

	sprintf(buffer1, "%s\\MyImg\\", buffer1);


	// 文件夹不存在则创建文件夹
	if (_access(buffer1, 0) == -1)
	{
		_mkdir(buffer1);
	}

	/*filename.Format("%s\\%d%.02d%.02d%.02d%.02d%.02d%s",
		buffer1,
		ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec,
		".jpg");*/

	filename.Format("%s%s%s", buffer1, "img", ".jpg");


	//https://docs.microsoft.com/zh-cn/windows/win32/api/wingdi/
	//https://docs.microsoft.com/en-us/cpp/atl-mfc-shared/reference/cimage-class?view=vs-2015
	//https://docs.microsoft.com/zh-cn/windows/win32/api/wingdi/nf-wingdi-bitblt	

	HDC hdcSrc = ::GetDC(NULL);
	int nBitPerPixel = GetDeviceCaps(hdcSrc, BITSPIXEL);
	int nWidth = GetDeviceCaps(hdcSrc, HORZRES);
	int nHeight = GetDeviceCaps(hdcSrc, VERTRES);

	CImage image;
	image.Create(nWidth, nHeight, nBitPerPixel);

	BitBlt(image.GetDC(), 0, 0, nWidth, nHeight, hdcSrc, 0, 0, SRCCOPY);
	::ReleaseDC(NULL, hdcSrc);

	HRESULT saveJPEG = image.Save(filename, Gdiplus::ImageFormatJPEG);

	image.ReleaseDC();
	image.Destroy();

	if (S_OK == saveJPEG)
	{
		//图片上加文字戳
		string strStl = filename.GetBuffer(0);
		if (SetImaeTextDate(strStl))
		{
			BOOL dimage = ::DeleteFile(filename);
			if (!dimage)
			{
				DWORD error = GetLastError();
				char buf[1024] = { 0 };
				sprintf_s(buf, _countof(buf), "DeleteFile %s Error:%d", filename, error);
				LOG(buf);
			}
		}
	}
	else
	{
		LOG(string("saveJPEG Error") + filename.GetBuffer());
	}
	return 0;
}

//以下是读写图片的调用代码：
bool ReadPhotoFile(std::basic_string<TCHAR> strFileName, std::string &strData)
{
	HANDLE hFile;
	hFile = CreateFile(strFileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dFileSize = GetFileSize(hFile, NULL);
	char * pBuffer = new char[dFileSize + 1];

	if (pBuffer == NULL)
		return false;

	memset(pBuffer, 0, dFileSize);

	DWORD dReadSize(0);
	if (!ReadFile(hFile, pBuffer, dFileSize, &dReadSize, NULL))
	{
		LOG("ReadFile Error" + strFileName);
		delete[]pBuffer;
		CloseHandle(hFile);
		return false;
	}

	CBase64 base64;
	strData = "";
	if (!base64.Encode((const char*)pBuffer, dReadSize, strData))
	{
		LOG("base64.Encode Error");
	}

	delete[] pBuffer;
	CloseHandle(hFile);
	return true;
}

bool WritePhotoFile(std::basic_string<TCHAR> strFileName, std::string &strData)
{
	HANDLE hFile;
	hFile = CreateFile(strFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	CBase64 base64;
	int datalen(0);
	DWORD dwritelen(0);
	std::string strdcode;
	base64.Decode(strData.data(), strData.size(), strdcode);
	if (!WriteFile(hFile, strdcode.data(), datalen, &dwritelen, NULL))
	{
		CloseHandle(hFile);
		return false;
	}
	CloseHandle(hFile);
	return true;
}


int aide(bool force)
{
	std::lock_guard<std::mutex> lock_a(IDPManager::User.AIDALock);
	//如果非强制性刷新，检测
	if (!force)
	{
		time_t now = time(NULL);
		if ((now - IDPManager::mLastAIDA) / 60 < IDPManager::Config.aidaSpac)
		{
			LOG_DAILY("在两次AIDA的间隔中重复调用AIDA，未发生调用");
			return -1;
		}
	}
	else
	{
		LOG_DAILY("强制调用AIDA");
	}
	//从当前目录找到文件
	stdstr AIDAPath = PathHelper::GetModuleFolder() + _T("aida64business\\aida64.exe");
	if (FileManager::IsFileExist(AIDAPath.c_str()))//判断文件是否有效 TRUE 有效
	{
		IDPManager::mLastAIDA = time(NULL);
		LOG_DAILY("调用" + AIDAPath);
		// /R 不弹窗口出来	/ALL 完整报告	/SUM 仅系统摘要	/HW	硬件相关内容 /SW 软件相关内容 /BENCH 性能测试内容 /AUDIT 监视内容 /CUSTOM 自定义选择
		//打开aida采集数据 这个时间有点长，会等待程序采集结束后才会向下执行
		ForkUtils::ShellExec(AIDAPath, _T("/R /HW /SILENT /INI"), true);
	}
	else
	{
		LOG("调用失败：" + AIDAPath);
		return -1;
	}
	return 0;
}


void OnCommandCollAll()
{
	try
	{
		//采集硬件数据 ==========================================
		Json::Value instructRoot;
		BUILD_MSG_HEAD(instructRoot);
		instructRoot["commandType"] = (int)COMMAND::COMM_COLLALL;
		instructGlobal.clear();
		//加锁	关闭指令 1 全部发送情况重复
		if (instructGlobalIF)
		{
			instructGlobalIF = false;

			//第二个参数用来判断是否增加到全局json里面  只有在发送1指令全部时候才设置true增加，单指令false
			instructScreenCapture(true);	//屏幕截图  
			SendDiskInfo(true);			//磁盘
			instructCreateMemory(true);	//内存
			instructScreenInfo(true);	//屏幕信息
			instructPrinter(true);		//打印机
			SendKeyMouse(true);		//键盘鼠标
			SendCPUInfo(true);
			SendMotherInfo(true);
			SendUSB(true, true);
			instructRoot["acquisitionData"] = instructGlobal;
			coutn++;

			instructGlobalIF = true;
		}

		if (coutn == 1)
		{
			RocketMQLink(instructRoot, "instruct"); //参数1 是显示在界面属于哪个标签
			coutn--;
		}
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}

//收集全部硬件信息
void OnCommandHardware(bool bLaunch)
{
	try
	{
		LOG_DAILY("OnCommandHardware Begin");
		Json::Value instructRoot;
		BUILD_MSG_HEAD(instructRoot);
		instructRoot["commandType"] = (int)COMMAND::COMM_HARDWARE;
		//清空数据
		instructGlobal.clear();

		//加锁	关闭指令 1 全部发送情况重复
		if (instructGlobalIF)
		{
			instructGlobalIF = false;

			//第二个参数用来判断是否增加到全局json里面  只有在发送1指令全部时候才设置true增加，单指令false
			SendDiskInfo(true);			//磁盘
			instructCreateMemory(true);	//内存
			instructScreenInfo(true);	//屏幕信息
			instructPrinter(true);		//打印机
			SendKeyMouse(true);		//键盘鼠标
			SendCPUInfo(true);				//CPU
			SendMotherInfo(true);			//主板
			SendUSB(true, true);
			instructRoot["acquisitionData"] = instructGlobal;

			coutn++;
			instructGlobalIF = true;
		}
		if (coutn == 1)
		{
			if (bLaunch)
			{
				RocketMQLink(instructRoot, "device");
			}
			else
			{
				RocketMQLink(instructRoot, "instruct");
			}
			coutn--;
		}
		LOG_DAILY("OnCommandHardware End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}

//有事件发生，主动发送
void OnEventSend()
{
	try
	{
		LOG_DAILY("Send event begin");
		std::lock_guard<std::mutex> lock_a(IDPManager::User.MsgLock);
		Json::Value instructRoot;
		BUILD_MSG_HEAD(instructRoot);
		instructRoot["commandType"] = (int)COMMAND::COMM_EVENT;
		//清空数据
		instructGlobal.clear();
		//加锁	关闭指令 1 全部发送情况重复
		if (instructGlobalIF)
		{
			instructGlobalIF = false;
			//第二个参数用来判断是否增加到全局json里面  只有在发送1指令全部时候才设置true增加，单指令false
			SendUSB(true, true);
			instructRoot["acquisitionData"] = instructGlobal;
			coutn++;
			instructGlobalIF = true;
		}
		if (coutn == 1)
		{
			RocketMQLink(instructRoot, "event");
			coutn--;
		}
		LOG_DAILY("Send event end");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
//采集截图功能
void instructScreenCapture(bool ifSortSent)
{
	try
	{

		LOG_DAILY("instructScreenCapture Begin");
		Json::Value instructRoot;

		//截图功能 转换base64发送
		// 1 屏幕截图
		GetWindowScreen2();

		// 2 图片转换base64
		char buffer2[MAX_PATH] = { 0 };
		DWORD code = GetEnvironmentVariable("USERPROFILE", buffer2, MAX_PATH);

		string buf1(buffer2);
		buf1 += "\\MyImg\\";

		char buffer3[MAX_PATH] = { 0 };

		sprintf(buffer3, _TEXT("%s"), "img");


		buf1 += buffer3;
		buf1 += "W.jpg";
		string data;
		ReadPhotoFile(buf1, data);
		Json::Value wmiScreenCapture;
		wmiScreenCapture["pic"] = data;
		auto str = wmiScreenCapture["pic"].toStyledString();
		if (str.size() < data.size())
		{
			LOG("instructScreenCapture dataSize" + to_string(data.size()) + " But jsonData Size " + to_string(str.size()));
		}
		else
		{
			LOG_DAILY("instructScreenCapture dataSize" + to_string(data.size()) + " JsonData Size " + to_string(str.size()));
		}

		instructRoot["acquisitionData"].append(wmiScreenCapture);
		instructRoot["commandType"] = (int)COMMAND::COMM_SCREEN;
		if (ifSortSent)
		{
			instructGlobal.append(instructRoot);//用来发送全部指令数据
		}



		if (instructGlobalIF)//这里增加判断是为了调用全部数据时候不用重复发送
		{
			BUILD_MSG_HEAD(instructRoot);

			RocketMQLink(instructRoot, "instruct");
		}
		LOG_DAILY("instructScreenCapture End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}


#define BUFFERSIZE 255


void instructCreateMemory(bool ifSortSent)
{
	try
	{
		LOG_DAILY("instructCreateMemory Begin");
		Json::Value instructRoot;

		//WMI  内存信息
		Json::Value wmiPrintf;
		Json::Value wmiMemory;

		CHAR *filename = "C:\\VEIDP\\aida64business\\Reports\\Report.ini";

		char tval[MAX_PATH] = { 0 };
		Json::Value root;
		Json::Value root2;
		int count = 0;//计数
		for (int i = 1; i < 10; i++)
		{
			char str0[1024] = { "Default" };
			sprintf(str0, "内存设备%d|内存设备|大小", i);
			char str1[1024] = { 0 };
			::GetPrivateProfileString("DMI", str0, "*", tval, MAX_PATH, filename);

			if (strcmp(tval, "*") == 0)
			{
				break;
			}
			cout << ++count << endl;
		}


		for (int i = 1; i <= count; i++)
		{
			char str2[1024] = { 0 };
			sprintf(str2, "内存设备%d|内存设备|制造商", i);
			::ZeroMemory(tval, MAX_PATH);
			::GetPrivateProfileString("DMI", str2, "*", tval, MAX_PATH, filename);
			root["brand"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);

			sprintf(str2, "内存设备%d|内存设备|类型", i);
			::GetPrivateProfileString("DMI", str2, "*", tval, MAX_PATH, filename);
			root["series"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);

			sprintf(str2, "内存设备%d|内存设备|大小", i);
			::GetPrivateProfileString("DMI", str2, "*", tval, MAX_PATH, filename);
			root["size"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);

			sprintf(str2, "内存设备%d|内存设备|最大时钟频率", i);
			::GetPrivateProfileString("DMI", str2, "*", tval, MAX_PATH, filename);
			root["memoryMZ"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);


			sprintf(str2, "内存设备%d|内存设备|序列号", i);
			::GetPrivateProfileString("DMI", str2, "*", tval, MAX_PATH, filename);
			root["SerialNumber"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);
#if __TEST_VM
#else
			instructRoot["acquisitionData"].append(root);
#endif
		}
		if (instructRoot["acquisitionData"].isNull())
		{
			instructRoot["acquisitionData"].append("");
		}
		instructRoot["commandType"] = (int)COMMAND::COMM_MEMORY;
		if (ifSortSent)
		{
			instructGlobal.append(instructRoot);//用来发送全部指令数据
		}

		if (instructGlobalIF)																	//这里增加判断是为了调用全部数据时候不用重复发送
		{
			BUILD_MSG_HEAD(instructRoot);
			RocketMQLink(instructRoot, "instruct");
		}
		LOG_DAILY("instructCreateMemory End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}

void instructScreenInfo(bool ifSortSent)
{
	LOG_DAILY("instructScreenInfo Begin");
	try
	{
		Json::Value instructRoot;

		CHAR *filename = "C:\\VEIDP\\aida64business\\Reports\\Report.ini";

		char tval[MAX_PATH] = { 0 };
		Json::Value root;
		int count = 0;//计数
		for (int i = 1; i < 10; i++)
		{
			char str1[1024] = { 0 };
			sprintf(str1, "显示器%d|显示器属性|显示器名称", i);
			::GetPrivateProfileString("显示器", str1, "*", tval, MAX_PATH, filename);

			if (strcmp(tval, "*") == 0)
			{
				break;
			}
			count++;
		}
		std::string wallpaper;
		for (int i = 0; i < 10; i++)
		{
			//屏幕壁纸
			std::wstring wstr;
			auto hr = GetWallpaper(wstr);
			if (wstr.empty())
			{
				LOG_DAILY(std::string("GetWallpaper Error ") + ErrorUtils::ComErrorMessage(hr) + ",will use wmi interface");
				std::string err;
				wallpaper = IDPManager::WMI().GetDesktopWallpaper(err);				//偶现报错，暂停使用
				wallpaper = CONVERT(wallpaper);
			}
			else
			{
				wallpaper = CppConvert::UnicodeToUTF8(wstr);
			}
			if (!wallpaper.empty())
			{
				break;
			}
			Sleep(500);
		}


		for (int i = 1; i <= count; i++)
		{
			char str2[1024] = { 0 };
			sprintf(str2, "显示器%d|显示器属性|显示器名称", i);
			::ZeroMemory(tval, MAX_PATH);
			::GetPrivateProfileString("显示器", str2, "*", tval, MAX_PATH, filename);
			root["brand"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);

			sprintf(str2, "显示器%d|显示器属性|最大分辨率", i);
			::GetPrivateProfileString("显示器", str2, "*", tval, MAX_PATH, filename);
			root["imageSize"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);

			sprintf(str2, "显示器%d|显示器属性|最大可用屏幕", i);
			::GetPrivateProfileString("显示器", str2, "*", tval, MAX_PATH, filename);
			root["size"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);

			sprintf(str2, "显示器%d|显示器属性|型号", i);
			::GetPrivateProfileString("显示器", str2, "*", tval, MAX_PATH, filename);
			root["model"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);

			sprintf(str2, "显示器%d|显示器属性|序列号", i);
			::GetPrivateProfileString("显示器", str2, "*", tval, MAX_PATH, filename);
			root["SerialNumber"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);
#if __TEST_VM
#else
			root["backImage"] = wallpaper;

			instructRoot["acquisitionData"].append(root);
#endif
		}
		if (instructRoot["acquisitionData"].isNull())
		{
			instructRoot["acquisitionData"].append("");
		}
		instructRoot["commandType"] = (int)COMMAND::COMM_MONIT;
		if (ifSortSent)
		{
			instructGlobal.append(instructRoot);//用来发送全部指令数据
		}
		if (instructGlobalIF)//这里增加判断是为了调用全部数据时候不用重复发送
		{
			BUILD_MSG_HEAD(instructRoot);
			RocketMQLink(instructRoot, "instruct");
		}
		LOG_DAILY("instructScreenInfo End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}

void instructPrinter(bool ifSortSent)
{
	try
	{
		LOG_DAILY("instructPrinter Begin");
		Json::Value instructRoot;


		CHAR *filename = "C:\\VEIDP\\aida64business\\Reports\\Report.ini";

		char tval[MAX_PATH] = { 0 };
		Json::Value root;
		int count = 0;//计数
		for (int i = 1; i < 10; i++)
		{
			char str1[1024] = { 0 };
			sprintf(str1, "打印机%d|打印机属性|打印机名称", i);
			::GetPrivateProfileString("打印机", str1, "*", tval, MAX_PATH, filename);

			if (strcmp(tval, "*") == 0)
			{
				break;
			}
			count++;
		}

		for (int i = 1; i <= count; i++)
		{
			char str2[1024] = { 0 };
			sprintf(str2, "打印机%d|打印机属性|打印机名称", i);
			::ZeroMemory(tval, MAX_PATH);
			::GetPrivateProfileString("打印机", str2, "*", tval, MAX_PATH, filename);
			root["name"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);

			sprintf(str2, "打印机%d|打印机属性|打印机驱动程序", i);
			::GetPrivateProfileString("打印机", str2, "*", tval, MAX_PATH, filename);
			root["driver"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);

			sprintf(str2, "打印机%d|打印机属性|默认打印机", i);
			::GetPrivateProfileString("打印机", str2, "*", tval, MAX_PATH, filename);
			root["isDefault"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);

			sprintf(str2, "打印机%d|打印机属性|共享", i);
			::GetPrivateProfileString("打印机", str2, "*", tval, MAX_PATH, filename);
			root["isShare"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);

			sprintf(str2, "打印机%d|打印机属性|状态", i);
			::GetPrivateProfileString("打印机", str2, "*", tval, MAX_PATH, filename);
			root["status"] = CONVERT(tval);
			::ZeroMemory(tval, MAX_PATH);
#if __TEST_VM
#else
			instructRoot["acquisitionData"].append(root);
#endif
		}
		if (instructRoot["acquisitionData"].isNull())
		{
			instructRoot["acquisitionData"].append("");
		}
		instructRoot["commandType"] = (int)COMMAND::COMM_PRINTER;
		if (ifSortSent)
		{
			instructGlobal.append(instructRoot);//用来发送全部指令数据
		}

		if (instructGlobalIF)//这里增加判断是为了调用全部数据时候不用重复发送
		{
			BUILD_MSG_HEAD(instructRoot);

			RocketMQLink(instructRoot, "instruct");
		}
		LOG_DAILY("instructPrinter End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}

//=============== 软件打开关闭计数 ================


// =============== 软件打开关闭计数 ================*/
//BOOL g_IsWork = FALSE;
//HANDLE g_hThread;
//string g_boot_time;
//string g_countIniPath;
//map<string, process_info> g_processList;
//fnNtQuerySystemInformation NtQuerySystemInformation = NULL;

string GetBootTime()
{
	off_on ofon;
	ofon.NtQuerySystemInformation = (fnNtQuerySystemInformation)GetProcAddress(LoadLibrary("ntdll.dll"), "NtQuerySystemInformation");
	if (ofon.NtQuerySystemInformation == NULL)
	{
		return "";
	}

	LONG status;
	SYSTEM_TIME_INFORMATION sti;
	status = ofon.NtQuerySystemInformation(SystemTimeInformation, &sti, sizeof(sti), 0);
	if (NO_ERROR != status)
	{
		return "";
	}

	FILETIME ft;
	SYSTEMTIME st;
	memcpy(&ft, &sti.liKeBootTime, sizeof(ft));
	FileTimeToLocalFileTime(&ft, &ft);
	FileTimeToSystemTime(&ft, &st);

	char timeBuf[MAX_PATH] = { 0 };
	sprintf(timeBuf, "%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond);

	return timeBuf;
}


void SendDiskInfo(bool ifSortSent)
{
	try
	{
		LOG_DAILY("SendDiskInfo Begin");
		Json::Value instructRoot;


		//WMI  磁盘信息
		iniManager ini("C:\\VEIDP\\aida64business\\Reports\\Report.ini");

		std::vector<IDPDisk> diskvec;
		std::vector<IDPDrive> drivec;
		BOOL bRet = HardWareUtils::GetIDPDisk(IDPManager::WMI(), ini, drivec, diskvec);
		//统计数据
		Json::Value diskRoot;
		for (int i = 0; i < diskvec.size(); i++)
		{
			auto& info = diskvec[i];
			Json::Value js;
			js["name"] = CONVERT(info.name);
			js["totalSize"] = to_string(info.totalSize);
			js["residueSize"] = to_string(info.residueSize);
#if __TEST_VM
#else
			diskRoot["disk"].append(js);
#endif
		}
		if (diskRoot.isNull())
		{
			diskRoot["disk"] = "";
		}
		instructRoot["acquisitionData"].append(diskRoot);

		Json::Value drivecRoot;
		for (int i = 0; i < drivec.size(); i++)
		{
			auto& info = drivec[i];
			Json::Value js;
			js["totalSize"] = to_string(info.totalSize) + "G";
			js["brand"] = Convert::string_To_UTF8(info.brand.c_str());
			js["InterfaceType"] = Convert::string_To_UTF8(info.InterfaceType);
			js["rev"] = Convert::string_To_UTF8(info.rev);
			js["SerialNumber"] = Convert::string_To_UTF8(info.SerialNumber);
#if __TEST_VM
#else
			drivecRoot["drivec"].append(js);
#endif
		}
		if (drivecRoot.isNull())
		{
			drivecRoot["drivec"] = "";
		}
		instructRoot["acquisitionData"].append(drivecRoot);
		instructRoot["commandType"] = (int)COMMAND::COMM_DISK;
		if (ifSortSent)
		{
			instructGlobal.append(instructRoot);												//用来发送全部指令数据
		}

		if (instructGlobalIF)																				//这里增加判断是为了调用全部数据时候不用重复发送
		{
			BUILD_MSG_HEAD(instructRoot);

			RocketMQLink(instructRoot, "instruct");											//这里标签是显示在界面的
		}
		LOG_DAILY("SendDiskInfo End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}

void SendCPUInfo(bool ifSortSent)
{
	try
	{
		LOG_DAILY("SendCPUInfo Begin");
		Json::Value instructRoot;

		iniManager ini("C:\\VEIDP\\aida64business\\Reports\\Report.ini");

		std::vector<IDPCPU> vec;
		BOOL bRet = HardWareUtils::GetIDPCPU(ini, vec);
		if (bRet == FALSE)
		{
			LOG("HardWareUtils::GetIDPCPU Error");
		}

		//统计数据
		for (int i = 0; i < vec.size(); i++)
		{
			auto& info = vec[i];
			Json::Value js;
			js["brand"] = CONVERT(info.brand);
			js["series"] = CONVERT(info.series);
			js["cpuT"] = CONVERT(info.cpuT);
			js["cpuFanR"] = CONVERT(info.cpuFanR);
			js["Tjmax"] = CONVERT(info.Tjmax);
			//容易异常的数据，缓存
			if (!info.cpuid.empty())
			{
				IDPManager::User.CPUID = info.cpuid;
			}
			js["cpuid"] = CONVERT(IDPManager::User.CPUID);
#if __TEST_VM
#else
			instructRoot["acquisitionData"].append(js);
#endif
		}
		if (instructRoot["acquisitionData"].isNull())
		{
			instructRoot["acquisitionData"].append("");
		}
		instructRoot["commandType"] = (int)COMMAND::COMM_CPU;
		if (ifSortSent)
		{
			instructGlobal.append(instructRoot);												//用来发送全部指令数据
		}

		if (instructGlobalIF)																				//这里增加判断是为了调用全部数据时候不用重复发送
		{
			BUILD_MSG_HEAD(instructRoot);
			RocketMQLink(instructRoot, "instruct");											//这里标签是显示在界面的
		}
		LOG_DAILY("SendCPUInfo End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
void SendMotherInfo(bool ifSortSent)
{
	try
	{
		LOG_DAILY("SendMotherInfo Begin");
		Json::Value instructRoot;

		iniManager ini("C:\\VEIDP\\aida64business\\Reports\\Report.ini");

		std::vector<IDPMother> vec;
		BOOL bRet = HardWareUtils::GetIDPMother(ini, vec);
		if (bRet == FALSE)
		{
			LOG("HardWareUtils::GetIDPMother Error");
		}
		//统计数据
		for (int i = 0; i < vec.size(); i++)
		{
			auto& info = vec[i];
			Json::Value js;
			js["brand"] = CONVERT(info.brand);
			js["series"] = CONVERT(info.series);
			js["name"] = CONVERT(info.name);
			js["mainboardCore"] = CONVERT(info.mainboardCore);
			js["mainboardT"] = CONVERT(info.mainboardT);
			js["busBandwidth"] = CONVERT(info.busBandwidth);
#if __TEST_VM
#else
			instructRoot["acquisitionData"].append(js);
#endif
		}
		if (instructRoot["acquisitionData"].isNull())
		{
			instructRoot["acquisitionData"].append("");
		}
		instructRoot["commandType"] = (int)COMMAND::COMM_MAINBOARD;
		if (ifSortSent)
		{
			instructGlobal.append(instructRoot);												//用来发送全部指令数据
		}

		if (instructGlobalIF)																				//这里增加判断是为了调用全部数据时候不用重复发送
		{
			BUILD_MSG_HEAD(instructRoot);
			RocketMQLink(instructRoot, "instruct");											//这里标签是显示在界面的
		}
		LOG_DAILY("SendMotherInfo End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
void SendKeyMouse(bool ifSortSent)
{
	try
	{
		LOG_DAILY("SendKeyMouse Begin");
		Json::Value instructRoot;

		iniManager ini("C:\\VEIDP\\aida64business\\Reports\\Report.ini");

		std::vector<IDPKeyboard> keyVec;
		BOOL bRet = HardWareUtils::GetKeyboard(ini, keyVec);
		if (bRet == FALSE)
		{
			LOG("HardWareUtils::GetKeyboard Error");
		}
		std::vector<IDPMouse> mouseVec;
		bRet = HardWareUtils::GetMouse(ini, mouseVec);
		if (bRet == FALSE)
		{
			LOG("HardWareUtils::GetMouse Error");
		}
		//统计数据
		Json::Value mouseRoot;
		for (int i = 0; i < mouseVec.size(); i++)
		{
			auto& info = mouseVec[i];
			Json::Value js;
			js["name"] = CONVERT(info.name);
			js["isExistRoller"] = CONVERT(info.isExistRoller);
			js["rollingLine"] = to_string(info.rollingLine);
#if __TEST_VM
#else
			mouseRoot["mouse"].append(js);
#endif
		}
		if (mouseRoot.isNull())
		{
			mouseRoot["mouse"] = "";
		}
		instructRoot["acquisitionData"].append(mouseRoot);

		Json::Value keyRoot;
		for (int i = 0; i < keyVec.size(); i++)
		{
			auto& info = keyVec[i];
			Json::Value js;
			js["name"] = CONVERT(info.name);
			js["type"] = CONVERT(info.type);
			js["keyboardInterface"] = CONVERT(info.keyboardInterface);
#if __TEST_VM
#else
			keyRoot["keyboard"].append(js);
#endif
		}
		if (keyRoot.isNull())
		{
			keyRoot["keyboard"] = "";
		}
		instructRoot["acquisitionData"].append(keyRoot);

		instructRoot["commandType"] = (int)COMMAND::COMM_KEY_MOUSE;
		if (ifSortSent)
		{
			instructGlobal.append(instructRoot);												//用来发送全部指令数据
		}

		if (instructGlobalIF)															//这里增加判断是为了调用全部数据时候不用重复发送
		{
			BUILD_MSG_HEAD(instructRoot);
			RocketMQLink(instructRoot, "instruct");											//这里标签是显示在界面的
		}
		LOG_DAILY("SendKeyMouse End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}

void SendUSB(bool bUpdate, bool ifSortSend)
{
	try
	{
		LOG_DAILY("SendUSB Begin");
		Json::Value instructRoot;
		std::map<stdstr, USBDevice> usbmap;
		if (bUpdate)
		{
			usbmap = IDPManager::User.UpdateUsbMap();
		}
		else
		{
			usbmap = IDPManager::User.GetUsbMap();
		}

		//统计数据
		Json::Value usbRoot;
		for (auto it = usbmap.begin(); it != usbmap.end(); it++)
		{
			Json::Value js;
			js["description"] = CONVERT(it->second.Description);
			js["busReported"] = Convert::UnicodeToUtf8(it->second.BusReportedDeviceDescription);
#if __TEST_VM
#else
			usbRoot["usb"].append(js);
#endif
		}
		if (usbRoot.isNull())
		{
			usbRoot["usb"] = "";
		}
		instructRoot["acquisitionData"].append(usbRoot);

		instructRoot["commandType"] = (int)COMMAND::COMM_USB;
		if (ifSortSend)
		{
			instructGlobal.append(instructRoot);												//用来发送全部指令数据
		}

		if (instructGlobalIF)															//这里增加判断是为了调用全部数据时候不用重复发送
		{
			BUILD_MSG_HEAD(instructRoot);
			RocketMQLink(instructRoot, "instruct");											//这里标签是显示在界面的
		}
		LOG_DAILY("SendUSB End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}