#include "utils.h"
#include "tlhelp32.h"
#pragma comment(lib, "WS2_32")

UtilsTools::UtilsTools()
{
}

UtilsTools::~UtilsTools()
{
}

//判断一个进程是否启动
bool UtilsTools::IsProcessRun(const char *pName)
{

	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	//DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return FALSE;
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		// Retrieve the priority class.
		//dwPriorityClass = 0;
		if (!strcmp(pName, cstringClass.CSting2PChar(pe32.szExeFile)))
		{
			return TRUE;

			//HANDLE hGUI = OpenProcess(PROCESS_ALL_ACCESS, true, pe32.th32ProcessID);
			//if (NULL == hGUI) {
			//	std::cout << "获取进程句柄失败" << std::endl;
			//	break;
			//}
			//TerminateProcess(hGUI, 0);  //终止进程
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return FALSE;
}

//启动一个进程
BOOL UtilsTools::CallCommand(char *cmd, char *parameters)
{
	LPWSTR cmdStr = cstringClass.Str2Unicode(cmd);
	LPWSTR parametersStr = cstringClass.Str2Unicode(parameters);
	SHELLEXECUTEINFO  ShExecInfo = { 0 };

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = cmdStr;
	ShExecInfo.lpParameters = parametersStr;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;

	return ShellExecuteEx(&ShExecInfo);
	//WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}

net_info UtilsTools::GetNetInfo()
{
	net_info  result;
	ULONG buffer_size = 0;
	GetAdaptersInfo(NULL, &buffer_size);
	PIP_ADAPTER_INFO p_ip_info = (IP_ADAPTER_INFO *)malloc(buffer_size);
	GetAdaptersInfo(p_ip_info, &buffer_size);
	while (p_ip_info)
	{
		ULONG gate_way = GetHostOrder(p_ip_info->GatewayList.IpAddress.String);
		IP_ADDR_STRING* p_ip_item = &p_ip_info->IpAddressList;
		bool is_right_ip = false;
		while (p_ip_item)
		{
			ULONG tpIp = GetHostOrder(p_ip_item->IpAddress.String);
			result.strIp = p_ip_item->IpAddress.String;

			ULONG net_mask = GetHostOrder(p_ip_item->IpMask.String);
			ULONG sub_net = gate_way & net_mask;
			ULONG sub_net_x = tpIp & net_mask;
			if (tpIp != 0)
			{
				if (sub_net == sub_net_x)
				{
					is_right_ip = true;
					break;
				}
			}
			p_ip_item = p_ip_item->Next;
		}

		if (is_right_ip)
		{
			UCHAR mac[MAX_PATH] = { 0 };
			int iCount = 0;

			for (int i = 0; i<(int)p_ip_info->AddressLength; i++)
			{
				Byte2Hex(p_ip_info->Address[i], &mac[iCount]);
				iCount += 2;
				if (i<(int)p_ip_info->AddressLength - 1)
				{
					mac[iCount++] = ':';
				}
			}

			result.strMac = (char*)mac;

			break;
		}

		p_ip_info = p_ip_info->Next;
	}

	return result;
}

ULONG UtilsTools::GetHostOrder(const string strIp)
{
	ULONG iIp = inet_addr(strIp.c_str());
	iIp = ntohl(iIp);
	return iIp;
}

VOID UtilsTools::Byte2Hex(unsigned char bData, unsigned char hex[])
{
	int high = bData / 16, low = bData % 16;
	hex[0] = (high <10) ? ('0' + high) : ('A' + high - 10);
	hex[1] = (low <10) ? ('0' + low) : ('A' + low - 10);
}

string UtilsTools::ltos(unsigned long l)
{
	ostringstream os;
	os << l;
	string result;
	istringstream is(os.str());
	is >> result;
	return result;

}

// 获得本机中处理器的数量
int UtilsTools::_GetNoOfProcessors()
{
	SYSTEM_INFO si;

	GetSystemInfo(&si);

	return si.dwNumberOfProcessors;
}

string UtilsTools::get_application_dir()
{
	char file_path[260];
	GetModuleFileNameA(NULL, file_path, sizeof(file_path));
	::PathRemoveFileSpecA(file_path);
	return file_path;
}

tm UtilsTools::get_current_time()
{
	time_t current_time_1 = time(NULL);
	tm current_time_2;
	localtime_s(&current_time_2, &current_time_1);
	return current_time_2;
}

BOOL UtilsTools::CreateMultiDirectory(CString strPath)
{
	CString strSubPath;
	CString strMsg;
	int nCount = 0;
	int nIndex = 0;
	//通过“\”来分割路径，从而创建各级的目录。
	do
	{
		nIndex = strPath.Find(_T("/"), nIndex) + 1;
		nCount++;
	} while ((nIndex - 1) != -1);
	nIndex = 0;
	//检查，并创建目录
	while ((nCount - 1) >= 0)
	{
		nIndex = strPath.Find(_T("/"), nIndex) + 1;
		if ((nIndex - 1) == -1)
			strSubPath = strPath;
		else
			strSubPath = strPath.Left(nIndex);

		if (!PathFileExists(strSubPath))// - 检查目录是否存在
		{
			if (!CreateDirectory(strSubPath, NULL))// -不存在则创建目录
			{
				return FALSE;
			}
		}
		nCount--;
	};
	return TRUE;
}
//void showSystemCodePage() {
//	
//	switch ((int)GetACP())
//	{
//	case 936:
//		//puts("ANSI/OEM GBK");
//		break;
//	case 65001:
//
//		break;
//	default:
//		break;
//	}
//}