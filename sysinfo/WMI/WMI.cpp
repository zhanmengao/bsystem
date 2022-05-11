#include"WMI.h"
#include"../../../../C++space/StringTools/StringTools/StrUtils.h"
#include"../../../../C++space/Convert/Convert/Convert.h"

bool CWMI::InitWMI()
{
	HRESULT hres; //定义COM调用的返回  
	bool bRet = false;

	try {
		hres = CoInitializeEx(0, COINIT_MULTITHREADED);
		if (FAILED(hres))
		{
			throw exception("CoInitializeEx() error.");
		}

		hres = CoInitializeSecurity(
			NULL,
			-1,                          // COM authentication  
			NULL,                        // Authentication services  
			NULL,                        // Reserved  
			RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication   
			RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation    
			NULL,                        // Authentication info  
			EOAC_NONE,                   // Additional capabilities   
			NULL                         // Reserved  
		);

		if (FAILED(hres))
		{
			throw exception("CoInitializeEx() error.");
		}

		hres = CoCreateInstance(
			CLSID_WbemLocator,
			0,
			CLSCTX_INPROC_SERVER,
			IID_IWbemLocator, (LPVOID *)&pLoc_);

		if (FAILED(hres))
		{
			throw exception("CoCreateInstance() error.");
		}

		// to make IWbemServices calls.  
		hres = pLoc_->ConnectServer(
			_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace  
			NULL,                    // User name. NULL = current user  
			NULL,                    // User password. NULL = current  
			0,                       // Locale. NULL indicates current  
			NULL,                    // Security flags.  
			0,                       // Authority (e.g. Kerberos)  
			0,                       // Context object   
			&pSvc_                    // pointer to IWbemServices proxy  
		);

		if (FAILED(hres))
		{
			throw exception("ConnectServer() error.");
		}

		hres = CoSetProxyBlanket(
			pSvc_,                        // Indicates the proxy to set  
			RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx  
			RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx  
			NULL,                        // Server principal name   
			RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx   
			RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx  
			NULL,                        // client identity  
			EOAC_NONE                    // proxy capabilities   
		);

		if (FAILED(hres))
		{
			throw exception("CoSetProxyBlanket() error.");
		}


	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		return bRet;
	}

	bRet = true;
	return bRet;
}

bool CWMI::ClearWMI()
{
	bool bRet = false;

	if (NULL != pSvc_)
	{
		pSvc_->Release();
		pSvc_ = NULL;
	}

	if (pLoc_ != NULL)
	{
		pLoc_->Release();
		pLoc_ = NULL;
	}
	CoUninitialize();
	bRet = true;
	return bRet;
}
bool CWMI::QuerySystemInfo()
{
	HRESULT hres; //定义COM调用的返回  
	IEnumWbemClassObject* pEnumerator = NULL;
	bool bRet = false;

	try
	{
		hres = pSvc_->ExecQuery(
			bstr_t("WQL"),
			bstr_t("SELECT * FROM Win32_OperatingSystem"),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&pEnumerator);

		if (FAILED(hres))
		{
			throw exception("ExecQuery() error.");
		}

		while (pEnumerator)
		{
			IWbemClassObject *pclsObj;
			ULONG uReturn = 0;

			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
				&pclsObj, &uReturn);
			if (0 == uReturn)
			{
				break;
			}

			GetInfo(L"BootDevice", pclsObj);
			GetInfo(L"Caption", pclsObj);
			GetInfo(L"Manufacturer", pclsObj);
			GetInfo(L"CSName", pclsObj);
			GetInfo(L"WindowsDirectory", pclsObj);
			GetInfo(L"SystemDirectory", pclsObj);
			GetInfo(L"TotalVisibleMemorySize", pclsObj);
			GetInfo(L"FreePhysicalMemory", pclsObj);

			pclsObj->Release();
		}


	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		if (pEnumerator != NULL)
		{
			pEnumerator->Release();
			pEnumerator = NULL;
		}
		return bRet;
	}


	if (pEnumerator != NULL)
	{
		pEnumerator->Release();
		pEnumerator = NULL;
	}

	bRet = true;
	return bRet;
}



WMIQuery CWMI::GetInfo(const WCHAR* wszQueryInfo, IWbemClassObject *pclsObj)
{
	WMIQuery ret;
	if (wszQueryInfo == NULL || NULL == pclsObj)
		return ret;
	VARIANT vtProp;
	CIMTYPE pType;
	char* lpszText = NULL;

	HRESULT hr = pclsObj->Get(wszQueryInfo, 0, &vtProp, &pType, 0);
	if (SUCCEEDED(hr))
	{
		ret.dataType = vtProp.vt;
		switch (vtProp.vt)
		{
		case VT_BSTR:
		{
			lpszText = _com_util::ConvertBSTRToString(V_BSTR(&vtProp));
			if (lpszText)
			{
				StrUtils::_strcpy(ret.data.data, sizeof(ret.data), lpszText);
				delete[] lpszText;
				lpszText = NULL;
			}
		}
		break;
		case VT_I2:
		{
			short vs = V_I2(&vtProp);
			ret.data.dwData = vs;
		}
		break;
		case VT_I4:
		{
			int vi = V_I4(&vtProp);
			ret.data.dwData = vi;
			break;
		}
		case VT_BOOL:
		{
			bool vi = V_BOOL(&vtProp);
			ret.data.bVal = vi;
			break;
		}
		default:
			break;
		}
		VariantClear(&vtProp);
	}
	return ret;
}
void CWMI::PrintWMIVec(const std::vector<WMIQueryData>& rData)
{
	for (unsigned i = 0; i < rData.size(); i++)
	{
		printf("\nItem %u--------------------------------------\n", i);
		for (auto it = rData[i].begin(); it != rData[i].end(); it++)
		{
			auto &data = it->second;
			if (data.dataType == VT_I2 || data.dataType == VT_I4)
			{
				printf("%ws	%d  %u\n", it->first.c_str(), data.dataType, data.data.dwData);
			}
			else if (data.dataType == VT_BOOL)
			{
				printf("%ws	%d  %d\n", it->first.c_str(), data.dataType, data.data.bVal);
			}
			else
			{
				printf("%ws	%d  %s\n", it->first.c_str(), data.dataType, data.data.data);
			}
		}
	}
}
HRESULT CWMI::Query(char* wql, std::vector<WMIQueryData>& rData)
{
	rData.clear();
	HRESULT hres = S_OK;
	hres = CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	if (FAILED(hres))
	{
		cout << "Failed to initialize COM library. Error code = 0x" << hex << hres << endl;
		return hres;
	}

	hres = CoInitializeSecurity(NULL, -1, NULL, NULL, 
		RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 
		EOAC_NONE, NULL);
	if (FAILED(hres))
	{
		cout << "Failed to initialize security. Error code = 0x" << hex << hres << endl;
		CoUninitialize();
		return hres;
	}

	IWbemLocator* pLoc = 0;
	hres = CoCreateInstance(
		CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
	if (FAILED(hres))
	{
		cout << "Failed to create IWbemLocator object. Err code = 0x" << hex << hres << endl;
		CoUninitialize();
		return hres;
	}

	IWbemServices* pSvc = 0;

	hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
	if (FAILED(hres))
	{
		cout << "Could not connect. Error code = 0x" << hex << hres << endl;
		pLoc->Release();
		CoUninitialize();
		return hres;
	}

	hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

	if (FAILED(hres))
	{
		cout << "Could not set proxy blanket. Error code = 0x" << hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return hres;
	}
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(bstr_t("WQL"), bstr_t(wql),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
	if (FAILED(hres))
	{
		cout << "Query for print queues failed. Error code = 0x" << hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return hres;
	}
	else
	{
		IWbemClassObject* pInstance = NULL;
		ULONG dwCount;

		while (pEnumerator->Next(WBEM_INFINITE, 1, &pInstance, &dwCount) == S_OK)
		{
			WMIQueryData info;
			SAFEARRAY* pvNames = NULL;
			if (pInstance->GetNames(NULL,
				WBEM_FLAG_ALWAYS | WBEM_MASK_CONDITION_ORIGIN, NULL, &pvNames) == S_OK)
			{
				if (pvNames == NULL)
				{
					continue;
				}
				long vbl, vbu;
				SafeArrayGetLBound(pvNames, 1, &vbl);
				SafeArrayGetUBound(pvNames, 1, &vbu);
				for (long idx = vbl; idx <= vbu; idx++)
				{
					long aidx = idx;
					wchar_t* wsName = 0;
					SafeArrayGetElement(pvNames, &aidx, &wsName);
					if (wsName != NULL)
					{
						auto data = GetInfo(wsName, pInstance);
						info.insert(std::make_pair(wsName, data));
						SysFreeString(wsName);
						wsName = NULL;
					}
				}
			}
			if (pvNames)
			{
				rData.push_back(info);
				SafeArrayDestroy(pvNames);
				pvNames = NULL;
			}
		}
		if (pInstance)
		{
			pInstance->Release();
			pInstance = NULL;
		}

	}
	if (pEnumerator)
	{
		pEnumerator->Release();
		pEnumerator = NULL;
	}
	if (pSvc)
	{
		pSvc->Release();
		pSvc = NULL;
	}
	CoUninitialize();
	return hres;
}



//系统补丁
HRESULT   CWMI::GetPatchList(vector<WMIPatchInfo>& patchlist)
{
	HRESULT hRet = S_OK;

	hRet = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hRet))
	{
		return hRet;
	}

	IWbemLocator *pIWbemLocator = NULL;
	hRet = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pIWbemLocator);
	if (FAILED(hRet))
	{
		CoUninitialize();
		return 0;
	}

	IWbemServices *pIWbemServices = NULL;
	bstr_t strNetwoekResource("ROOT\\CIMV2");
	hRet = pIWbemLocator->ConnectServer(strNetwoekResource, NULL, NULL, NULL, 0, NULL, NULL, &pIWbemServices);
	if (FAILED(hRet))
	{
		pIWbemLocator->Release();
		CoUninitialize();
		return hRet;
	}

	hRet = CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (FAILED(hRet))
	{
		pIWbemServices->Release();
		pIWbemLocator->Release();
		CoUninitialize();
		return hRet;
	}

	bstr_t strQueryLanguage("WQL");
	bstr_t strQuery("SELECT * FROM Win32_QuickFixEngineering");

	IEnumWbemClassObject *pIEnumWbemClassObject = NULL;

	hRet = pIWbemServices->ExecQuery(strQueryLanguage, strQuery,
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pIEnumWbemClassObject);

	if (SUCCEEDED(hRet))
	{
		do
		{
			ULONG uReturned = 0;
			IWbemClassObject *pIWbemClassObject = NULL;

			hRet = pIEnumWbemClassObject->Next(WBEM_INFINITE, 1, &pIWbemClassObject, &uReturned);
			if (SUCCEEDED(hRet) && pIWbemClassObject)
			{
				WMIPatchInfo fixInfo;

				VARIANT vtName, vtDescription, vtInstallDate;
				hRet = pIWbemClassObject->Get(L"HotFixID", 0, &vtName, NULL, NULL);
				char* pTmpStr = NULL;

				if (SUCCEEDED(hRet))
				{
					fixInfo.Name = vtName.bstrVal;
					delete[] pTmpStr;
					VariantClear(&vtName);

					if (!WinStrUtils::Compareni(fixInfo.Name.c_str(), L"KB", 2))
					{
						continue;
					}
				}

				hRet = pIWbemClassObject->Get(L"Description", 0, &vtDescription, NULL, NULL);
				if (SUCCEEDED(hRet))
				{
					pTmpStr = _com_util::ConvertBSTRToString(vtDescription.bstrVal);

					if (pTmpStr)
					{
#ifdef _UNICODE
						fixInfo.Description = Convert::StringToWString(pTmpStr);
#else
						fixInfo.Description = pTmpStr;
#endif
						delete[] pTmpStr;
						pTmpStr = NULL;
					}
					VariantClear(&vtDescription);
					//映射
					if (fixInfo.Description == _T("Update"))
						fixInfo.Description = _T("更新补丁");
					else if (fixInfo.Description == _T("Hotfix"))
						fixInfo.Description = _T("修复补丁");
					else if (fixInfo.Description == _T("Security Update"))
						fixInfo.Description = _T("安全补丁");
				}

				hRet = pIWbemClassObject->Get(L"InstalledOn", 0, &vtInstallDate, NULL, NULL);
				if (SUCCEEDED(hRet))
				{
					char* pInstallDate = _com_util::ConvertBSTRToString(vtInstallDate.bstrVal);
					int wYear, wMonth, wDay;
					sscanf_s(pInstallDate, "%02d/%02d/%04d",&wMonth, &wDay, &wYear);
					TCHAR strTime[100] = { 0 };
					wsprintf(strTime, L"%04d-%02d-%02d", wYear, wMonth, wDay);

					fixInfo.installYear = wYear;
					fixInfo.installMonth = wMonth;
					fixInfo.installDay = wDay;
					if (WinStrUtils::Comparei(strTime, L"Invalid DateTime"))
					{
						//fixInfo.Install_time = L"";
					}
					else
					{
						//fixInfo.install_time = strTime;
					}
					if (pInstallDate)
					{
						delete[] pInstallDate;
						pInstallDate = NULL;
					}
					VariantClear(&vtInstallDate);
				}
				patchlist.push_back(fixInfo);
			}
			else
			{
				break;
			}
		} while (TRUE);
	}

	pIWbemServices->Release();
	pIWbemLocator->Release();

	CoUninitialize();

	return S_OK;
}
HRESULT CWMI::GetMonitorProcess(off_on& ofon)
{
	HRESULT hRet = S_OK;
	IWbemLocator *pIWbemLocator = NULL;
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	hRet = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pIWbemLocator);
	if (FAILED(hRet))
	{
		CoUninitialize();
		return 0;
	}

	IWbemServices *pIWbemServices = NULL;
	bstr_t strNetwoekResource("ROOT\\CIMV2");
	hRet = pIWbemLocator->ConnectServer(strNetwoekResource, NULL, NULL, NULL, 0, NULL, NULL, &pIWbemServices);
	if (FAILED(hRet))
	{
		pIWbemLocator->Release();
		CoUninitialize();
		return hRet;
	}

	hRet = CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, 
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (FAILED(hRet))
	{
		pIWbemServices->Release();
		pIWbemLocator->Release();
		CoUninitialize();
		return hRet;
	}

	bstr_t strQueryLanguage("WQL");
	bstr_t strQuery("SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'");

	IEnumWbemClassObject *pIEnumWbemClassObject = NULL;

	hRet = pIWbemServices->ExecNotificationQuery(strQueryLanguage, strQuery, WBEM_FLAG_FORWARD_ONLY | 
		WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pIEnumWbemClassObject);

	if (SUCCEEDED(hRet))
	{
		do
		{
			ULONG uReturned = 0;
			IWbemClassObject *pIWbemClassObject = NULL;

			hRet = pIEnumWbemClassObject->Next(2000, 1, &pIWbemClassObject, &uReturned);

			if (SUCCEEDED(hRet) && pIWbemClassObject)
			{
				VARIANT vtInstanceObject;
				hRet = pIWbemClassObject->Get(L"TargetInstance", 0, &vtInstanceObject, NULL, NULL);

				if (SUCCEEDED(hRet) && vtInstanceObject.vt == VT_UNKNOWN && vtInstanceObject.punkVal != NULL)
				{
					IWbemClassObject *pTargetInstance = (IWbemClassObject*)vtInstanceObject.punkVal;

					VARIANT vtProcessID, vtExecutablePath, vtExecutableName;

					hRet = pTargetInstance->Get(L"ProcessID", 0, &vtProcessID, NULL, NULL);
					if (SUCCEEDED(hRet))
					{
						// 获取进程名  
						hRet = pTargetInstance->Get(L"Name", 0, &vtExecutableName, NULL, NULL);
						if (SUCCEEDED(hRet))
						{
							printf("pid  = %d  name %s", vtProcessID.intVal, _com_util::ConvertBSTRToString(vtExecutableName.bstrVal));
							//DWORD processId = vtProcessID.iVal;
							//HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processId);
							//if (hProcess == NULL)
							//{
							//	VariantClear(&vtProcessID);
							//	continue;
							//}
							//VariantClear(&vtProcessID);

							//DWORD ret = GetGuiResources(hProcess, GR_USEROBJECTS);
							//if (ret > 1)
							//{
							//	char* pName = _com_util::ConvertBSTRToString(vtExecutableName.bstrVal);
							//	if (stricmp(pName, "conhost.exe") != 0 &&
							//		stricmp(pName, "dllhost.exe") != 0)
							//	{
							//		pTargetInstance->Get(L"ExecutablePath", 0, &vtExecutablePath, NULL, NULL);
							//		char* pExePath = _com_util::ConvertBSTRToString(vtExecutablePath.bstrVal);
							//		stdstr fileVersion = GetFileVersion(Convert::StringToWString(pExePath));

							//		PathRemoveExtensionA(pName);

							//		if (ofon.g_processList.count(pName) < 1 || ofon.g_processList[pName].version != fileVersion)
							//		{
							//			process_info info;
							//			info.name = pName;
							//			info.version = fileVersion;
							//			info.count = 0;
							//			ofon.g_processList[pName] = info;
							//		}

							//		ofon.g_processList[pName].count++;

							//		WriteProcessIni(ofon.g_processList[pName]);

							//		delete[] pExePath;
							//		VariantClear(&vtExecutablePath);
							//	}
							//	delete[] pName;
							//	VariantClear(&vtExecutableName);
							//}
							//CloseHandle(hProcess);
						}
					}

					vtInstanceObject.punkVal->Release();
				}
			}

		} while (TRUE && ofon.g_IsWork);
	}

	pIWbemServices->Release();
	pIWbemLocator->Release();

	return 0;
}
stdstr CWMI::GetFileVersion(const stdstr& exePath)
{
	DWORD dwVerInfoSize = 0;
	DWORD dwVerHnd;
	dwVerInfoSize = GetFileVersionInfoSize(exePath.c_str(), &dwVerHnd);
	if (dwVerInfoSize)
	{
		unsigned int uInfoSize = 0;
		BYTE *pData = NULL;
		VS_FIXEDFILEINFO * pFileInfo;

		pData = new BYTE[dwVerInfoSize + 1];
		ZeroMemory(pData, dwVerInfoSize + 1);
		GetFileVersionInfo(exePath.c_str(), 0, dwVerInfoSize, pData);
		VerQueryValue(pData, (LPTSTR)("\\"), (void**)&pFileInfo, &uInfoSize);

		WORD m_nProdVersion[4];
		m_nProdVersion[0] = HIWORD(pFileInfo->dwProductVersionMS);
		m_nProdVersion[1] = LOWORD(pFileInfo->dwProductVersionMS);
		m_nProdVersion[2] = HIWORD(pFileInfo->dwProductVersionLS);
		m_nProdVersion[3] = LOWORD(pFileInfo->dwProductVersionLS);

		TCHAR buffer[100] = { 0 };
		wsprintf(buffer, _T("%d.%d.%d.%d"), m_nProdVersion[0],
			m_nProdVersion[1], m_nProdVersion[2], m_nProdVersion[3]);

		delete[] pData;

		return buffer;
	}

	return _T("");
}