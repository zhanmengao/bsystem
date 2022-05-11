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

	//һ����ʼ��COM���
	//��ʼ��COM
	hr = ::CoInitializeEx(0, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr) || RPC_E_CHANGED_MODE == hr)
	{
		//���ý��̵İ�ȫ���𣬣�����COM���ʱ�ڳ�ʼ��COM֮��Ҫ����CoInitializeSecurity���ý��̰�ȫ���𣬷���ᱻϵͳʶ��Ϊ������
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

		//��������һ��WMI�����ռ�����
		//����һ��CLSID_WbemLocator����
		hr = CoCreateInstance(CLSID_WbemLocator,
			0,
			CLSCTX_INPROC_SERVER,
			IID_IWbemLocator,
			(LPVOID*)&m_pWbemLoc);
		VERIFY(SUCCEEDED(hr));

		//ʹ��m_pWbemLoc���ӵ�"root\cimv2"������m_pWbemSvc��ָ��
		hr = m_pWbemLoc->ConnectServer(CComBSTR(L"ROOT\\CIMV2"),
			NULL,
			NULL,
			0,
			NULL,
			0,
			0,
			&m_pWbemSvc);
		VERIFY(SUCCEEDED(hr));

		//��������WMI���ӵİ�ȫ��
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
		//��ѯ��ClassName�е������ֶ�,���浽m_pEnumClsObj��
		query += CComBSTR(ClassName);
		hr = m_pWbemSvc->ExecQuery(CComBSTR("WQL"), query, WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			0, &m_pEnumClsObj);
		if (SUCCEEDED(hr))
		{
			//��ʼ��vtPropֵ
			VariantInit(&vtProp);
			uReturn = 0;

			//���شӵ�ǰλ����ĵ�һ������m_pWbemClsObj��
			hr = m_pEnumClsObj->Next(WBEM_INFINITE, 1, &m_pWbemClsObj, &uReturn);
			if (SUCCEEDED(hr) && uReturn > 0)
			{
				//��m_pWbemClsObj���ҳ�ClassMember��ʶ�ĳ�Ա����ֵ,�����浽vtProp������
				hr = m_pWbemClsObj->Get(CComBSTR(ClassMember), 0, &vtProp, 0, 0);
				if (SUCCEEDED(hr))
				{
					VariantToString(&vtProp, chRetValue);
					VariantClear(&vtProp);//���vtProp
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
			VariantInit(&vtProp); //��ʼ��vtProp����
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
			chRetValue = "��";
		else
			chRetValue = "��";
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

//����̴��һ�����ɣ�WMI���ֶ�������������Win32_��ͷ���������磺
//Win32_Processor��Win32_PhysicalMemory�ȵȡ��������ԣ���ֻҪ��MSDN����Win32_�ͻῴ���ܶ���WMI��β������
//
//���������г�һЩ���õ�������
//
//// Ӳ����Ϣ����
//Win32_Processor, // CPU ������
//Win32_PhysicalMemory, // �����ڴ���
//Win32_Keyboard, // ����
//Win32_PointingDevice, // �������豸��������ꡣ
//Win32_FloppyDrive, // ����������
//Win32_DiskDrive, // Ӳ��������
//Win32_CDROMDrive, // ����������
//Win32_BaseBoard, // ����
//Win32_BIOS, // BIOS оƬ
//Win32_ParallelPort, // ����
//Win32_SerialPort, // ����
//Win32_SerialPortConfiguration, // ��������
//Win32_SoundDevice, // ��ý�����ã�һ��ָ������
//Win32_SystemSlot, // ������ (ISA & PCI & AGP)
//Win32_USBController, // USB ������
//Win32_NetworkAdapter, // ����������
//Win32_NetworkAdapterConfiguration, // ��������������
//Win32_Printer, // ��ӡ��
//Win32_PrinterConfiguration, // ��ӡ������
//Win32_PrintJob, // ��ӡ������
//Win32_TCPIPPrinterPort, // ��ӡ���˿�
//Win32_POTSModem, // MODEM
//Win32_POTSModemToSerialPort, // MODEM �˿�
//Win32_DesktopMonitor, // ��ʾ��
//Win32_DisplayConfiguration, // �Կ�
//Win32_DisplayControllerConfiguration, // �Կ�����
//Win32_VideoController, // �Կ�ϸ�ڡ�
//Win32_VideoSettings, // �Կ�֧�ֵ���ʾģʽ��
//
//					 // ����ϵͳ
//	Win32_TimeZone, // ʱ��
//	Win32_SystemDriver, // ��������
//	Win32_DiskPartition, // ���̷���
//	Win32_LogicalDisk, // �߼�����
//	Win32_LogicalDiskToPartition, // �߼��������ڷ�����ʼĩλ�á�
//	Win32_LogicalMemoryConfiguration, // �߼��ڴ�����
//	Win32_PageFile, // ϵͳҳ�ļ���Ϣ
//	Win32_PageFileSetting, // ҳ�ļ�����
//	Win32_BootConfiguration, // ϵͳ��������
//	Win32_ComputerSystem, // �������Ϣ��Ҫ
//	Win32_OperatingSystem, // ����ϵͳ��Ϣ
//	Win32_StartupCommand, // ϵͳ�Զ���������
//	Win32_Service, // ϵͳ��װ�ķ���
//	Win32_Group, // ϵͳ������
//	Win32_GroupUser, // ϵͳ���ʺ�
//	Win32_UserAccount, // �û��ʺ�
//	Win32_Process, // ϵͳ����
//	Win32_Thread, // ϵͳ�߳�
//	Win32_Share, // ����
//	Win32_NetworkClient, // �Ѱ�װ������ͻ���
//	Win32_NetworkProtocol, // �Ѱ�װ������Э��

//          bstr_t("SELECT * FROM Win32_SoundDevice"),                          // ��Ƶ�豸  
//          bstr_t("SELECT * FROM Win32_Product"),    
//          bstr_t("SELECT * FROM Win32_Registry"),                             // ע���  
//          bstr_t("SELECT * FROM Win32_OnBoardDevice"),                        // �����ϵ�Ƕ���豸  
//          bstr_t("SELECT * FROM Win32_MotherboardDevice"),                    // ĸ��  
//          bstr_t("SELECT * FROM Win32_BaseBoard"),                            // ����  
//          bstr_t("SELECT * FROM Win32_DMAChannel"),                           // DMAͨ��  
//          bstr_t("SELECT * FROM Win32_Bus"),                                  // ����  
//          bstr_t("SELECT * FROM Win32_BIOS"),                                 // BIOSϵͳ  
//          bstr_t("SELECT * FROM Win32_SystemBIOS"),  
//          bstr_t("SELECT * FROM Win32_Processor"),                            // ������  
//          bstr_t("SELECT * FROM Win32_SystemProcesses"),                      //   
//          bstr_t("SELECT * FROM Win32_Process"),                              // ϵͳ����  
//          bstr_t("SELECT * FROM Win32_Thread"),                               // ϵͳ�߳�  
//          bstr_t("SELECT * FROM Win32_AssociatedProcessorMemory"),            // CACHE  
//          bstr_t("SELECT * FROM Win32_CacheMemory"),                          // ���������ڴ�  
//          bstr_t("SELECT * FROM Win32_PhysicalMedia"),                        // ����ý����Ϣ (Ӳ��)  
//          bstr_t("SELECT * FROM Win32_LogicalDisk "),                         // �߼�������  
//          bstr_t("SELECT * FROM Win32_DiskDrive"),                            // ����������  
//          bstr_t("SELECT * FROM Win32_MemoryDevice"),                         // �ڴ��豸  
//          bstr_t("SELECT * FROM Win32_PhysicalMemoryArray"),                  // �����ڴ�����  
//          bstr_t("SELECT * FROM Win32_PhysicalMemoryLocation"),               // �����ڴ�λ��  
//          bstr_t("SELECT * FROM CIM_NumericSensor"),                          // ���ִ�����  
//          bstr_t("SELECT * FROM Win32_VoltageProbe"),                         // ���ִ�����   
//          bstr_t("SELECT * FROM Win32_TemperatureProbe"),                     // �¶ȴ�����  
//          bstr_t("SELECT * FROM Win32_CurrentProbe"),       
//          bstr_t("SELECT * FROM Win32_OperatingSystem"),                      // ����ϵͳ      
//          bstr_t("SELECT * FROM Win32_UserAccount"),                          // �û��˺�  
//          bstr_t("SELECT * FROM Win32_SerialPort"),                           // ���нӿ�  
//          bstr_t("SELECT * FROM Win32_ParallelPort"),                         // ���нӿ�  
//          bstr_t("SELECT * FROM Win32_SCSIController"),                       // С�ͼ����ϵͳ�ӿ�  
//          bstr_t("SELECT * FROM Win32_PortResource"),                         // I/O �˿�  
//          bstr_t("SELECT * FROM Win32_PNPDevice"),                            // ���弴���豸  
//          bstr_t("SELECT * FROM Win32_NetworkAdapter"),                       // ����������  
//          bstr_t("SELECT * FROM Win32_NetworkAdapterConfiguration"),  
//          bstr_t("SELECT * FROM Win32_NetworkAdapterSetting"),  
//          bstr_t("SELECT * FROM Win32_AssociatedBattery"),  
//          bstr_t("SELECT * FROM Win32_Battery"),                              // �ڲ����  
//          bstr_t("SELECT * FROM Win32_PortableBattery"),  
//          bstr_t("SELECT * FROM Win32_PowerManagementEvent"),  
//          bstr_t("SELECT * FROM Win32_UninterruptiblePowerSupply"),  
//          bstr_t("SELECT * FROM Win32_DriverForDevice"),  
//          bstr_t("SELECT * FROM Win32_Printer"),                              // ��ӡ��  
//          bstr_t("SELECT * FROM Win32_TCPIPPrinterPort"),  
//          bstr_t("SELECT * FROM Win32_POTSModem"),  
//          bstr_t("SELECT * FROM Win32_DesktopMonitor"),                       // ��ʾ��  
//			bstr_t("SELECT * FROM Win32_VideoController"),                      // �Կ�  
//          bstr_t("SELECT * FROM Win32_CDROMDrive"),  
//          bstr_t("SELECT * FROM Win32_Keyboard"),                             // ����  
//          bstr_t("SELECT * FROM Win32_AutochkSetting"),  
//          bstr_t("SELECT * FROM Win32_PointingDevice"),                       // ����豸����ꡢ������  
//          bstr_t("SELECT * FROM Win32_Fan"),                                  // ����  
//          bstr_t("SELECT * FROM Win32_WMISetting"),  
//          bstr_t("SELECT * FROM Win32_TimeZone"),       
//          bstr_t("SELECT * FROM Win32_Environment"),                          // ����·��  
//          bstr_t("SELECT * FROM Win32_QuotaSetting"),  
//          bstr_t("SELECT * FROM Win32_NetworkProtocol"),                      // ����װ������Э��  