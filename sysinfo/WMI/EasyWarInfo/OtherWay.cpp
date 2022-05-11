#include "OtherWay.h"
#include <iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")

COtherWay::COtherWay(void)
{
}

COtherWay::~COtherWay(void)
{
}

UINT COtherWay::GetOS( CString &csName, CString &csVersion)
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	//
	// If that fails, try using the OSVERSIONINFO structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.

		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return FALSE;
	}

	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:

		// Test for the product.

		if ( osvi.dwMajorVersion <= 4 )
		{
			//printf("Microsoft Windows NT ");
			csName = _T("Microsoft Windows NT ");
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
		{//printf ("Microsoft Windows 2000 ");
			csName = _T("Microsoft Windows 2000 ");
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		{//	printf ("Microsoft Windows XP ");
			csName = _T("Microsoft Windows XP ");
		}

		// Test for product type.

		if( bOsVersionInfoEx )
		{
			if ( osvi.wProductType == VER_NT_WORKSTATION )
			{
				if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
					//printf ( "Personal " );
					csName += _T(" Personal ");
				else
					//printf ( "Professional " );
					csName += _T(" Professional ");
			}

			else if ( osvi.wProductType == VER_NT_SERVER )
			{
				if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
					//printf ( "DataCenter Server " );
					csName += _T("DataCenter Server ");
				else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					//printf ( "Advanced Server " );
					csName += _T("Advanced Server ");
				else
					//printf ( "Server " );
					csName += _T("Server ");
			}
		}
		else
		{
			HKEY hKey;
			char szProductType[80];
			DWORD dwBufLen;

			RegOpenKeyEx( HKEY_LOCAL_MACHINE,
				_T("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"),
				0, KEY_QUERY_VALUE, &hKey );
			RegQueryValueEx( hKey, _T("ProductType"), NULL, NULL,
				(LPBYTE) szProductType, &dwBufLen);
			RegCloseKey( hKey );
			if ( _strcmpi( "WINNT", szProductType) == 0 )
				//printf( "Professional " );
				csName += _T(" Professional ");
			if ( _strcmpi( "LANMANNT", szProductType) == 0 )
				//printf( "Server " );
				csName += _T(" Server ");
			if ( _strcmpi( "SERVERNT", szProductType) == 0 )
				//printf( "Advanced Server " );
				csName += _T(" Advanced Server ");
		}

		// Display version, service pack (if any), and build number.

		if ( osvi.dwMajorVersion <= 4 )
		{
			csVersion.Format (_T("%d.%d %S (Build %d)"),
				osvi.dwMajorVersion,
				osvi.dwMinorVersion,
				osvi.szCSDVersion,
				osvi.dwBuildNumber & 0xFFFF);
		}
		else
		{ 
			csVersion.Format (_T("%S (Build %d)"),
				osvi.szCSDVersion,
				osvi.dwBuildNumber & 0xFFFF);
		}
		break;

	case VER_PLATFORM_WIN32_WINDOWS:

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			//printf ("Microsoft Windows 95 ");
			csName = _T("Microsoft Windows 95 ");
			if ( osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B' )
				//printf("OSR2 " );
				csName += _T("OSR2 ");
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			//printf ("Microsoft Windows 98 ");
			csName = _T("Microsoft Windows 98 ");
			if ( osvi.szCSDVersion[1] == 'A' )
				//printf("SE " );
				csName += _T("SE ");
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			//printf ("Microsoft Windows Me ");
			csName = _T("Microsoft Windows Me ");
		} 
		break;

	case VER_PLATFORM_WIN32s:
		csName = _T("Microsoft Win32s ");
		//printf ("Microsoft Win32s ");
		break;
	}
	return TRUE; 

}

BOOL COtherWay::GetNetworkAdaptersByIPHelper( vector<NetworkAdaptertdll> &vecMyList)
{
	vecMyList.clear();

	ULONG ulOutBufLen = 0;
	if(GetAdaptersInfo(NULL, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW )
	{
		int nDivisor = sizeof(IP_ADAPTER_INFO);
		if(sizeof(time_t) == 8)
			nDivisor -= 8;

		int nCount = ulOutBufLen / nDivisor;
		//PIP_ADAPTER_INFO pAdapterInfo = new IP_ADAPTER_INFO[nCount];

		PIP_ADAPTER_INFO pAdapterInfo = (PIP_ADAPTER_INFO) new char[ulOutBufLen];
		PIP_ADAPTER_INFO ppAdapterInfo = pAdapterInfo;

		if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS )
		{
			delete[] pAdapterInfo;
			return FALSE;
		}
		else
		{
			for(int i = 0; i<nCount; i++)
			{
				////////////////////////////////////////////////////////
				DWORD dwRet = NULL;
				HKEY hKey = NULL;
				HKEY hSubKey = NULL;
				HKEY hDesKey = NULL;
				HKEY hSrvKey = NULL;

				if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Enum\\PCI"), NULL, KEY_READ, &hKey))
				{
					UINT i = 0;
					TCHAR tcSubKey[MAX_PATH];
					ZeroMemory(tcSubKey, sizeof(tcSubKey));
					while (ERROR_SUCCESS == RegEnumKey(hKey, i++, tcSubKey, sizeof(tcSubKey)))
					{
						TCHAR tcSubValue[MAX_PATH];
						ZeroMemory(tcSubValue, sizeof(tcSubValue));
						_tcscat_s(tcSubValue, _countof(tcSubValue), _T("SYSTEM\\CurrentControlSet\\Enum\\PCI"));
						_tcscat_s(tcSubValue, _countof(tcSubValue), _T("\\"));
						_tcscat_s(tcSubValue, _countof(tcSubValue), tcSubKey);
						if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, tcSubValue, NULL, KEY_READ, &hSubKey))
						{
							//±£´æHardwareID
							TCHAR tcCrntNicHardWareId[MAX_PATH];
							ZeroMemory(tcCrntNicHardWareId, sizeof(tcCrntNicHardWareId));
							_stprintf_s(tcCrntNicHardWareId, _countof(tcCrntNicHardWareId), _T("%s"), tcSubValue);

							UINT k = 0;
							TCHAR tcDesKey[MAX_PATH];
							ZeroMemory(tcDesKey, sizeof(tcDesKey));
							while (ERROR_SUCCESS == RegEnumKey(hSubKey, k++, tcDesKey, sizeof(tcDesKey)))
							{
								TCHAR tcSubKey[MAX_PATH];
								ZeroMemory(tcSubKey, sizeof(tcSubKey));
								_stprintf_s(tcSubKey, _countof(tcSubKey), _T("%s\\%s"), tcSubValue, tcDesKey);

								if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, tcSubKey, NULL, KEY_READ, &hDesKey))
								{
									DWORD dType = REG_SZ;
									TCHAR tcTmp[MAX_PATH];
									ZeroMemory(tcTmp, sizeof(tcTmp));
									DWORD dSize = sizeof(tcTmp);
									if (ERROR_SUCCESS == RegQueryValueEx(hDesKey, _T("Class"), NULL, &dType, (LPBYTE)tcTmp, &dSize))
									{
										if (0 == _tccmp(tcTmp, _T("Net")))
										{
											DWORD dUserType = NULL;
											DWORD dUserSize = MAX_PATH;
											TCHAR tcNicClassPath[MAX_PATH] = {0};
											HKEY hClassKey = NULL;

											//1 Driver
											dUserType = REG_SZ;
											dUserSize = MAX_PATH;
											if (ERROR_SUCCESS != RegQueryValueEx(hDesKey, _T("Driver"), NULL, &dUserType, (LPBYTE)tcNicClassPath, &dUserSize))
												goto RlzExit;

											TCHAR tcNicDelClassPath[MAX_PATH] = {0};
											_stprintf_s(tcNicDelClassPath, _countof(tcNicDelClassPath), _T("SYSTEM\\CurrentControlSet\\Control\\Class\\%s"), tcNicClassPath);
											if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, tcNicDelClassPath, NULL, KEY_READ, &hClassKey))
											{
												DWORD dType = REG_SZ;
												char tcTmp[MAX_PATH];
												ZeroMemory(tcTmp, sizeof(tcTmp));
												DWORD dSize = sizeof(tcTmp);
												if (ERROR_SUCCESS == RegQueryValueExA(hClassKey, "NetCfgInstanceId", NULL, &dType, (LPBYTE)tcTmp, &dSize))
												{
													if(strcmp(ppAdapterInfo->AdapterName, tcTmp) == 0)
													{
														NetworkAdaptertdll stNetworkAdaptert;

														DWORD dwType = REG_SZ;
														ZeroMemory(stNetworkAdaptert.csDescription, sizeof(stNetworkAdaptert.csDescription));
														DWORD dwSize = sizeof(stNetworkAdaptert.csDescription);
														RegQueryValueExA(hClassKey, "DriverDesc", NULL, &dwType, (LPBYTE)stNetworkAdaptert.csDescription, &dwSize);

														ZeroMemory(stNetworkAdaptert.csManufacturer, sizeof(stNetworkAdaptert.csManufacturer));
														dwSize = sizeof(stNetworkAdaptert.csManufacturer);
														RegQueryValueExA(hClassKey, "ProviderName", NULL, &dwType, (LPBYTE)stNetworkAdaptert.csManufacturer, &dwSize);

														strcpy(stNetworkAdaptert.csName, stNetworkAdaptert.csDescription);
														strcpy(stNetworkAdaptert.csProductName, stNetworkAdaptert.csDescription);

														vecMyList.push_back(stNetworkAdaptert);
													}
												}
											}
										}//if
									}//if
								}//if
							}//while
						}//if
						else
						{
							goto RlzExit;
						}
					}//while
				}//if
				else
				{
					goto RlzExit;
				}

RlzExit:
				if (hKey) RegCloseKey(hKey);
				if (hDesKey) RegCloseKey(hDesKey);
				if (hSubKey) RegCloseKey(hSubKey);
				if (hSrvKey) RegCloseKey(hSrvKey);
				///////////////////////////////////////////////////////
				ppAdapterInfo = pAdapterInfo->Next;
			}
			delete[] pAdapterInfo;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL COtherWay::GetNetworkAdaptersBySNMP( vector<NetworkAdaptertdll> &vecMyList)
{
	return TRUE;
}