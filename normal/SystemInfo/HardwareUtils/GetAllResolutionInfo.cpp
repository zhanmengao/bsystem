#include "GetAllResolutionInfo.h"	
BOOL CMonitorList::GetMonitors(vector<MonitorInfo> &vec)								//获取显示器信息
{
	CEdid edidInfo;
	edidInfo.Connect();
	if (edidInfo.IsConnected() && edidInfo.GetMonitors(vec))
	{
		edidInfo.Disconnect();
		return TRUE;
	}
	return FALSE;
}
vector<HMONITOR> CMonitorList::hMonitorGroup;
/*static */int CALLBACK CMonitorList::MonitorEnumProc(HMONITOR hMonitor,
	HDC hdc,
	LPRECT lpRMonitor,
	LPARAM dwData)
{
	hMonitorGroup.push_back(hMonitor);
	return 1;
}

CMonitorList::CMonitorList()
{
	hMonitorGroup.clear();
}

CMonitorList::~CMonitorList()
{
	m_miGroup.clear();
}

int CMonitorList::EnumDevicePort()
{
	BOOL bFlag = FALSE;

	DWORD m_dwDevicePortNum;		//设备输出端口，包含虚拟端口

	m_dwDevicePortNum = 0;
	do
	{
		DISPLAY_DEVICE dd_temp;
		ZeroMemory(&dd_temp, sizeof(dd_temp));
		dd_temp.cb = sizeof(DISPLAY_DEVICE);
		bFlag = ::EnumDisplayDevices(NULL, m_dwDevicePortNum, &dd_temp, 0);

		if (bFlag)
		{
			if (!((dd_temp.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) == DISPLAY_DEVICE_MIRRORING_DRIVER))//判断虚拟接口
			{
				RECT rcTemp = { 0, 0, 0, 0 };

				st_MI miTemp;
				memset(&miTemp, 0, sizeof(st_MI));
				miTemp.mi_DeviceActive = FALSE;
				miTemp.mi_DevicePrimary = FALSE;
				miTemp.mi_rcWork = rcTemp;

				miTemp.mi_DeviceName = dd_temp.DeviceName;

				miTemp.mi_DeviceString = dd_temp.DeviceString;

				miTemp.mi_DeviceKey = dd_temp.DeviceKey;

				miTemp.mi_DeviceID = dd_temp.DeviceID;

				miTemp.mi_StateFlags = dd_temp.StateFlags;
				if ((dd_temp.StateFlags & DISPLAY_DEVICE_ACTIVE) == DISPLAY_DEVICE_ACTIVE)						//判断接口激活状态
					miTemp.mi_DeviceActive = TRUE;
				m_miGroup.push_back(miTemp);
			}
			m_dwDevicePortNum++;
		}
	} while (bFlag);

	return 1;
}

int CMonitorList::EnumMonitor()
{
	::EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0);

	for (vector<HMONITOR>::iterator itehMoniter = hMonitorGroup.begin(); itehMoniter != hMonitorGroup.end(); itehMoniter++)
	{
		MONITORINFOEX mixTemp;
		memset(&mixTemp, 0, sizeof(MONITORINFOEX));
		mixTemp.cbSize = sizeof(MONITORINFOEX);

		GetMonitorInfo(*itehMoniter, &mixTemp);
		for (vector<st_MI>::iterator iteMI = m_miGroup.begin(); iteMI != m_miGroup.end(); iteMI++)
		{
			//char cTemp[32];
			//WideCharToMultiByte(CP_ACP, 0, mixTemp.szDevice, -1, cTemp, sizeof(cTemp), NULL, NULL);
			//strcpy(mixTemp.szDevice, cTemp);
			if (_tcscmp(iteMI->mi_DeviceName.c_str(), mixTemp.szDevice) == 0)
			{
				iteMI->mi_rcWork = mixTemp.rcWork;
				if ((iteMI->mi_StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) == DISPLAY_DEVICE_PRIMARY_DEVICE)//判断主显示器
					iteMI->mi_DevicePrimary = TRUE;

				HDC hdcMonitor = CreateDC(TEXT("Display"), mixTemp.szDevice, NULL, NULL);
				iteMI->mi_nColorDepth = GetDeviceCaps(hdcMonitor, BITSPIXEL) * GetDeviceCaps(hdcMonitor, PLANES);
				DeleteDC(hdcMonitor);

				DEVMODE dm_temp;
				memset(&dm_temp, 0, sizeof(DEVMODE));
				dm_temp.dmSize = sizeof(DEVMODE);

				EnumDisplaySettings(mixTemp.szDevice, ENUM_CURRENT_SETTINGS, &dm_temp);
				iteMI->mi_dmDisplayFrequency = dm_temp.dmDisplayFrequency;
				iteMI->mi_dmPelsWidth = dm_temp.dmPelsWidth;
				iteMI->mi_dmPelsHeight = dm_temp.dmPelsHeight;

				iteMI->mi_Id = ExactNumicFromString(iteMI->mi_DeviceName.c_str());  // 保存从设备名字符串后面提取出来的数字

				break;
			}
		}
	}

	return 1;
}


// 得到所有显示器的模式
void CMonitorList::GetAllDisplayMode()
{

	BOOL		bRetVal;
	DEVMODE 	 devmode;

	{
		vector<st_MI>::iterator itBeg = m_miGroup.begin();
		vector<st_MI>::iterator itEnd = m_miGroup.end();
		for (NULL; itBeg != itEnd; ++itBeg)
		{
			// 获取当前显示器正在使用的分辨率
			bRetVal = ::EnumDisplaySettings(itBeg->mi_DeviceName.c_str(), ENUM_CURRENT_SETTINGS, &devmode);
			if (bRetVal)
			{
				// 保存当前正在使用的分辨率宽和高
				itBeg->m_CurUsedResolution.m_nWidth = devmode.dmPelsWidth;
				itBeg->m_CurUsedResolution.m_nHeight = devmode.dmPelsHeight;
			}

			// =========================================================================		
			{ // 枚举显示器所有能支持的分辨率
				int iMode = 0;
				int cnts = 0; // 已经存储的分辨率种数
				do
				{
					bRetVal = ::EnumDisplaySettings(itBeg->mi_DeviceName.c_str(), iMode, &devmode);

					if (bRetVal)
					{
						bool bFind = false;
						for (int i = 0; i < cnts; ++i)
						{
							if (
								(itBeg->m_AllSupportResolution[i].m_nWidth == devmode.dmPelsWidth) &&
								(itBeg->m_AllSupportResolution[i].m_nHeight == devmode.dmPelsHeight)
								)
							{
								bFind = true;
							}
						}


						// 如果没有找到,才添加
						if (!bFind)
						{
							itBeg->m_AllSupportResolution[cnts].m_nWidth = devmode.dmPelsWidth;
							itBeg->m_AllSupportResolution[cnts].m_nHeight = devmode.dmPelsHeight;
							cnts++;
						}

					}

					iMode++;
				} while (bRetVal);

				itBeg->m_AllSupportSize = cnts;

				// 对支持的分辨率进行排序,从大到小(按系统那里的设置"分辨率" 是宽优先比较)
				for (int i = 0; i < cnts - 1; ++i)  // 冒泡法
				{
					for (int j = i + 1; j < cnts; ++j)
					{
						// i 的宽小于j的宽  或者 i的宽等于j的宽且i的高小于j的高
						if (
							(itBeg->m_AllSupportResolution[i].m_nWidth < itBeg->m_AllSupportResolution[j].m_nWidth) ||
							((itBeg->m_AllSupportResolution[i].m_nWidth == itBeg->m_AllSupportResolution[j].m_nWidth) && (itBeg->m_AllSupportResolution[i].m_nHeight < itBeg->m_AllSupportResolution[j].m_nWidth))
							)
						{
							// 交换数据
							int	tmpWidth, tmpHeight;

							// 交换宽
							tmpWidth = itBeg->m_AllSupportResolution[i].m_nWidth;
							itBeg->m_AllSupportResolution[i].m_nWidth = itBeg->m_AllSupportResolution[j].m_nWidth;
							itBeg->m_AllSupportResolution[j].m_nWidth = tmpWidth;

							// 交换高
							tmpHeight = itBeg->m_AllSupportResolution[i].m_nHeight;
							itBeg->m_AllSupportResolution[i].m_nHeight = itBeg->m_AllSupportResolution[j].m_nHeight;
							itBeg->m_AllSupportResolution[j].m_nHeight = tmpHeight;

						}

					}
				}


			}// ========================================================================= 		
		}
	}
}

vector<st_MI> CMonitorList::OutputMointorInfo()
{
	hMonitorGroup.clear();
	EnumDevicePort();
	EnumMonitor();
	GetAllDisplayMode();

	return m_miGroup;
}

// 修改指定设备分辨率
long CMonitorList::setDisplayRatio(LPCTSTR lpszDeviceName, int width, int height)
{
	DEVMODE lpDevmode;
	EnumDisplaySettings(NULL, 0, &lpDevmode);
	lpDevmode.dmPelsWidth = width;
	lpDevmode.dmPelsHeight = height;
	lpDevmode.dmSize = sizeof(lpDevmode);
	lpDevmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

	return ChangeDisplaySettingsEx(lpszDeviceName, &lpDevmode, NULL, CDS_UPDATEREGISTRY, NULL);
}

// 设置非主显示器分辨率，注意：是非主显示器（拼接屏）
void CMonitorList::setGlobalDisplayRatio(int width, int height)
{
	for (unsigned int i = 0; i < m_miGroup.size(); i++)
	{
		// 判断是否为已激活的非主显示器
		if (m_miGroup[i].mi_DeviceActive && (m_miGroup[i].mi_DevicePrimary == FALSE))
		{
			setDisplayRatio(m_miGroup[i].mi_DeviceName.c_str(), width, height);
		}
	}
}

// 设置显示器位置
long CMonitorList::setDisplayPosition(LPCTSTR lpszDeviceName, long x, long y)
{
	POINTL pos = { x, y };
	DEVMODE lpDevmode;
	EnumDisplaySettings(NULL, 0, &lpDevmode);
	lpDevmode.dmPosition = pos;
	lpDevmode.dmSize = sizeof(lpDevmode);
	lpDevmode.dmFields = DM_POSITION;
	return ChangeDisplaySettingsEx(lpszDeviceName, &lpDevmode, NULL, CDS_UPDATEREGISTRY, NULL);
}


// 功能：从字符串中提取最后的数字 ，如 "DISPLAY12", 提取出 12
// 参数： ch [in] ---- 源字符串
//        nCnts [in] ----- 字符串字符的个数
// 返回值：提取到的数值, 如果为未提取到数字；则返回 0
int CMonitorList::ExactNumicFromString(const TCHAR* ch)
{
	int nCnts = _tcslen(ch);

	if (0 == nCnts) return -1;

	while (nCnts > 0)
	{
		if (!_istdigit(ch[nCnts - 1]))
		{
			break;
		}

		nCnts--;
	}
	return _ttoi(ch + nCnts);
}