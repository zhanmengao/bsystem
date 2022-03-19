#include "CWMIInfo.h"

CWmiInfo::CWmiInfo(void)
{
	m_pWbemSvc = NULL;
	m_pWbemLoc = NULL;
	m_pEnumClsObj = NULL;
}

CWmiInfo::~CWmiInfo(void)
{
	m_pWbemSvc = NULL;
	m_pWbemLoc = NULL;
	m_pEnumClsObj = NULL;
}

HRESULT CWmiInfo::InitWmi()
{
	HRESULT hr;

	//一、初始化COM组件
	//初始化COM
	hr = ::CoInitializeEx(0, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr) || RPC_E_CHANGED_MODE == hr)
	{
		//设置进程的安全级别，（调用COM组件时在初始化COM之后要调用CoInitializeSecurity设置进程安全级别，否则会被系统识别为病毒）
		hr = CoInitializeSecurity(NULL,
			-1,
			NULL,
			NULL,
			RPC_C_AUTHN_LEVEL_PKT,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,
			EOAC_NONE,
			NULL);
		//VERIFY(SUCCEEDED(hr));

		//二、创建一个WMI命名空间连接
		//创建一个CLSID_WbemLocator对象
		hr = CoCreateInstance(CLSID_WbemLocator,
			0,
			CLSCTX_INPROC_SERVER,
			IID_IWbemLocator,
			(LPVOID*)&m_pWbemLoc);
		VERIFY(SUCCEEDED(hr));

		//使用m_pWbemLoc连接到"root\cimv2"并设置m_pWbemSvc的指针
		hr = m_pWbemLoc->ConnectServer(CComBSTR(L"ROOT\\CIMV2"),
			NULL,
			NULL,
			0,
			NULL,
			0,
			0,
			&m_pWbemSvc);
		VERIFY(SUCCEEDED(hr));

		//三、设置WMI连接的安全性
		hr = CoSetProxyBlanket(m_pWbemSvc,
			RPC_C_AUTHN_WINNT,
			RPC_C_AUTHZ_NONE,
			NULL,
			RPC_C_AUTHN_LEVEL_CALL,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,
			EOAC_NONE);
		VERIFY(SUCCEEDED(hr));
	}
	return(hr);
}

HRESULT CWmiInfo::ReleaseWmi()
{
	HRESULT hr;

	if (NULL != m_pWbemSvc)
	{
		hr = m_pWbemSvc->Release();
	}
	if (NULL != m_pWbemLoc)
	{
		hr = m_pWbemLoc->Release();
	}
	if (NULL != m_pEnumClsObj)
	{
		hr = m_pEnumClsObj->Release();
	}

	::CoUninitialize();

	return(hr);
}

BOOL CWmiInfo::GetSingleItemInfo(CString ClassName, CString ClassMember, CString &chRetValue)
{
	USES_CONVERSION;

	CComBSTR query("SELECT * FROM ");
	VARIANT vtProp;
	ULONG uReturn;
	HRESULT hr;
	BOOL bRet = FALSE;

	if (NULL != m_pWbemSvc)
	{
		//查询类ClassName中的所有字段,保存到m_pEnumClsObj中
		query += CComBSTR(ClassName);
		hr = m_pWbemSvc->ExecQuery(CComBSTR("WQL"), query, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			0, &m_pEnumClsObj);
		if (SUCCEEDED(hr))
		{
			//初始化vtProp值
			VariantInit(&vtProp);
			uReturn = 0;

			//返回从当前位置起的第一个对象到m_pWbemClsObj中
			hr = m_pEnumClsObj->Next(WBEM_INFINITE, 1, &m_pWbemClsObj, &uReturn);
			if (SUCCEEDED(hr) && uReturn > 0)
			{
				//从m_pWbemClsObj中找出ClassMember标识的成员属性值,并保存到vtProp变量中
				hr = m_pWbemClsObj->Get(CComBSTR(ClassMember), 0, &vtProp, 0, 0);
				if (SUCCEEDED(hr))
				{
					VariantToString(&vtProp, chRetValue);
					VariantClear(&vtProp);//清空vtProp
					bRet = TRUE;
				}
			}
		}
	}
	if (NULL != m_pEnumClsObj)
	{
		hr = m_pEnumClsObj->Release();
		m_pEnumClsObj = NULL;
	}
	if (NULL != m_pWbemClsObj)
	{
		hr = m_pWbemClsObj->Release();
		m_pWbemClsObj = NULL;
	}
	return bRet;
}

BOOL CWmiInfo::GetGroupItemInfo(CString ClassName, CString ClassMember[], int n, CString &chRetValue)
{
	USES_CONVERSION;

	CComBSTR query("SELECT * FROM ");
	CString result, info;
	VARIANT vtProp;
	ULONG uReturn;
	HRESULT hr;
	int i;
	BOOL bRet = FALSE;
	if (NULL != m_pWbemSvc)
	{
		query += CComBSTR(ClassName);
		hr = m_pWbemSvc->ExecQuery(CComBSTR("WQL"), query, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 0, &m_pEnumClsObj);
		if (SUCCEEDED(hr))
		{
			VariantInit(&vtProp); //初始化vtProp变量
			if (m_pEnumClsObj)
			{
				Sleep(10);
				uReturn = 0;
				hr = m_pEnumClsObj->Next(WBEM_INFINITE, 1, &m_pWbemClsObj, &uReturn);
				if (SUCCEEDED(hr) && uReturn > 0)
				{
					for (i = 0; i < n; ++i)
					{
						hr = m_pWbemClsObj->Get(CComBSTR(ClassMember[i]), 0, &vtProp, 0, 0);
						if (SUCCEEDED(hr))
						{
							VariantToString(&vtProp, info);
							chRetValue += info + _T("\t");
							VariantClear(&vtProp);
							bRet = TRUE;
						}
					}
					chRetValue += _T("\r\n");
				}
			}
		}
	}

	if (NULL != m_pEnumClsObj)
	{
		hr = m_pEnumClsObj->Release();
		m_pEnumClsObj = NULL;
	}
	if (NULL != m_pWbemClsObj)
	{
		hr = m_pWbemClsObj->Release();
		m_pWbemClsObj = NULL;
	}
	return bRet;
}

void CWmiInfo::VariantToString(const LPVARIANT pVar, CString &chRetValue) const
{
	USES_CONVERSION;

	CComBSTR HUGEP* pBstr;
	BYTE HUGEP* pBuf;
	LONG low, high, i;
	HRESULT hr;

	switch (pVar->vt)
	{
	case VT_BSTR:
	{
		chRetValue = W2T(pVar->bstrVal);
	}
	break;
	case VT_BOOL:
	{
		if (VARIANT_TRUE == pVar->boolVal)
			chRetValue = "是";
		else
			chRetValue = "否";
	}
	break;
	case VT_I4:
	{
		chRetValue.Format(_T("%d"), pVar->lVal);
	}
	break;
	case VT_UI1:
	{
		chRetValue.Format(_T("%d"), pVar->bVal);
	}
	break;
	case VT_UI4:
	{
		chRetValue.Format(_T("%d"), pVar->ulVal);
	}
	break;

	case VT_BSTR | VT_ARRAY:
	{
		hr = SafeArrayAccessData(pVar->parray, (void HUGEP**)&pBstr);
		hr = SafeArrayUnaccessData(pVar->parray);
		chRetValue = W2T(pBstr->m_str);
	}
	break;

	case VT_I4 | VT_ARRAY:
	{
		SafeArrayGetLBound(pVar->parray, 1, &low);
		SafeArrayGetUBound(pVar->parray, 1, &high);

		hr = SafeArrayAccessData(pVar->parray, (void HUGEP**)&pBuf);
		hr = SafeArrayUnaccessData(pVar->parray);
		CString strTmp;
		high = min(high, MAX_PATH * 2 - 1);
		for (i = low; i <= high; ++i)
		{
			strTmp.Format(_T("%02X"), pBuf[i]);
			chRetValue += strTmp;
		}
	}
	break;
	default:
		break;
	}
}

//这里教大家一个技巧，WMI的字段类名好像都是以Win32_开头。。。。如：
//Win32_Processor，Win32_PhysicalMemory等等。。。所以，你只要在MSDN输入Win32_就会看到很多以WMI结尾的类了
//
//下面给大家列出一些常用的类名：
//
//// 硬件信息类名
//Win32_Processor, // CPU 处理器
//Win32_PhysicalMemory, // 物理内存条
//Win32_Keyboard, // 键盘
//Win32_PointingDevice, // 点输入设备，包括鼠标。
//Win32_FloppyDrive, // 软盘驱动器
//Win32_DiskDrive, // 硬盘驱动器
//Win32_CDROMDrive, // 光盘驱动器
//Win32_BaseBoard, // 主板
//Win32_BIOS, // BIOS 芯片
//Win32_ParallelPort, // 并口
//Win32_SerialPort, // 串口
//Win32_SerialPortConfiguration, // 串口配置
//Win32_SoundDevice, // 多媒体设置，一般指声卡。
//Win32_SystemSlot, // 主板插槽 (ISA & PCI & AGP)
//Win32_USBController, // USB 控制器
//Win32_NetworkAdapter, // 网络适配器
//Win32_NetworkAdapterConfiguration, // 网络适配器设置
//Win32_Printer, // 打印机
//Win32_PrinterConfiguration, // 打印机设置
//Win32_PrintJob, // 打印机任务
//Win32_TCPIPPrinterPort, // 打印机端口
//Win32_POTSModem, // MODEM
//Win32_POTSModemToSerialPort, // MODEM 端口
//Win32_DesktopMonitor, // 显示器
//Win32_DisplayConfiguration, // 显卡
//Win32_DisplayControllerConfiguration, // 显卡设置
//Win32_VideoController, // 显卡细节。
//Win32_VideoSettings, // 显卡支持的显示模式。
//
//					 // 操作系统
//	Win32_TimeZone, // 时区
//	Win32_SystemDriver, // 驱动程序
//	Win32_DiskPartition, // 磁盘分区
//	Win32_LogicalDisk, // 逻辑磁盘
//	Win32_LogicalDiskToPartition, // 逻辑磁盘所在分区及始末位置。
//	Win32_LogicalMemoryConfiguration, // 逻辑内存配置
//	Win32_PageFile, // 系统页文件信息
//	Win32_PageFileSetting, // 页文件设置
//	Win32_BootConfiguration, // 系统启动配置
//	Win32_ComputerSystem, // 计算机信息简要
//	Win32_OperatingSystem, // 操作系统信息
//	Win32_StartupCommand, // 系统自动启动程序
//	Win32_Service, // 系统安装的服务
//	Win32_Group, // 系统管理组
//	Win32_GroupUser, // 系统组帐号
//	Win32_UserAccount, // 用户帐号
//	Win32_Process, // 系统进程
//	Win32_Thread, // 系统线程
//	Win32_Share, // 共享
//	Win32_NetworkClient, // 已安装的网络客户端
//	Win32_NetworkProtocol, // 已安装的网络协议

//          bstr_t("SELECT * FROM Win32_SoundDevice"),                          // 音频设备  
//          bstr_t("SELECT * FROM Win32_Product"),    
//          bstr_t("SELECT * FROM Win32_Registry"),                             // 注册表  
//          bstr_t("SELECT * FROM Win32_OnBoardDevice"),                        // 主板上的嵌入设备  
//          bstr_t("SELECT * FROM Win32_MotherboardDevice"),                    // 母板  
//          bstr_t("SELECT * FROM Win32_BaseBoard"),                            // 主板  
//          bstr_t("SELECT * FROM Win32_DMAChannel"),                           // DMA通道  
//          bstr_t("SELECT * FROM Win32_Bus"),                                  // 总线  
//          bstr_t("SELECT * FROM Win32_BIOS"),                                 // BIOS系统  
//          bstr_t("SELECT * FROM Win32_SystemBIOS"),  
//          bstr_t("SELECT * FROM Win32_Processor"),                            // 处理器  
//          bstr_t("SELECT * FROM Win32_SystemProcesses"),                      //   
//          bstr_t("SELECT * FROM Win32_Process"),                              // 系统进程  
//          bstr_t("SELECT * FROM Win32_Thread"),                               // 系统线程  
//          bstr_t("SELECT * FROM Win32_AssociatedProcessorMemory"),            // CACHE  
//          bstr_t("SELECT * FROM Win32_CacheMemory"),                          // 二级缓存内存  
//          bstr_t("SELECT * FROM Win32_PhysicalMedia"),                        // 物理媒体信息 (硬盘)  
//          bstr_t("SELECT * FROM Win32_LogicalDisk "),                         // 逻辑驱动器  
//          bstr_t("SELECT * FROM Win32_DiskDrive"),                            // 磁盘驱动器  
//          bstr_t("SELECT * FROM Win32_MemoryDevice"),                         // 内存设备  
//          bstr_t("SELECT * FROM Win32_PhysicalMemoryArray"),                  // 物理内存数组  
//          bstr_t("SELECT * FROM Win32_PhysicalMemoryLocation"),               // 物理内存位置  
//          bstr_t("SELECT * FROM CIM_NumericSensor"),                          // 数字传感器  
//          bstr_t("SELECT * FROM Win32_VoltageProbe"),                         // 数字传感器   
//          bstr_t("SELECT * FROM Win32_TemperatureProbe"),                     // 温度传感器  
//          bstr_t("SELECT * FROM Win32_CurrentProbe"),       
//          bstr_t("SELECT * FROM Win32_OperatingSystem"),                      // 操作系统      
//          bstr_t("SELECT * FROM Win32_UserAccount"),                          // 用户账号  
//          bstr_t("SELECT * FROM Win32_SerialPort"),                           // 串行接口  
//          bstr_t("SELECT * FROM Win32_ParallelPort"),                         // 并行接口  
//          bstr_t("SELECT * FROM Win32_SCSIController"),                       // 小型计算机系统接口  
//          bstr_t("SELECT * FROM Win32_PortResource"),                         // I/O 端口  
//          bstr_t("SELECT * FROM Win32_PNPDevice"),                            // 即插即用设备  
//          bstr_t("SELECT * FROM Win32_NetworkAdapter"),                       // 网络适配器  
//          bstr_t("SELECT * FROM Win32_NetworkAdapterConfiguration"),  
//          bstr_t("SELECT * FROM Win32_NetworkAdapterSetting"),  
//          bstr_t("SELECT * FROM Win32_AssociatedBattery"),  
//          bstr_t("SELECT * FROM Win32_Battery"),                              // 内部电池  
//          bstr_t("SELECT * FROM Win32_PortableBattery"),  
//          bstr_t("SELECT * FROM Win32_PowerManagementEvent"),  
//          bstr_t("SELECT * FROM Win32_UninterruptiblePowerSupply"),  
//          bstr_t("SELECT * FROM Win32_DriverForDevice"),  
//          bstr_t("SELECT * FROM Win32_Printer"),                              // 打印机  
//          bstr_t("SELECT * FROM Win32_TCPIPPrinterPort"),  
//          bstr_t("SELECT * FROM Win32_POTSModem"),  
//          bstr_t("SELECT * FROM Win32_DesktopMonitor"),                       // 显示器  
//			bstr_t("SELECT * FROM Win32_VideoController"),                      // 显卡  
//          bstr_t("SELECT * FROM Win32_CDROMDrive"),  
//          bstr_t("SELECT * FROM Win32_Keyboard"),                             // 键盘  
//          bstr_t("SELECT * FROM Win32_AutochkSetting"),  
//          bstr_t("SELECT * FROM Win32_PointingDevice"),                       // 点击设备：鼠标、触摸板  
//          bstr_t("SELECT * FROM Win32_Fan"),                                  // 风扇  
//          bstr_t("SELECT * FROM Win32_WMISetting"),  
//          bstr_t("SELECT * FROM Win32_TimeZone"),       
//          bstr_t("SELECT * FROM Win32_Environment"),                          // 环境路径  
//          bstr_t("SELECT * FROM Win32_QuotaSetting"),  
//          bstr_t("SELECT * FROM Win32_NetworkProtocol"),                      // 己安装的网络协议  