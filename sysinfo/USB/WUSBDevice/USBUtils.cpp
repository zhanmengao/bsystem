#include"USBUtils.h"

#ifdef DEFINE_DEVPROPKEY
#undef DEFINE_DEVPROPKEY
#endif
#ifdef INITGUID
#define DEFINE_DEVPROPKEY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8, pid) EXTERN_C const DEVPROPKEY DECLSPEC_SELECTANY name = { { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }, pid }
#else
#define DEFINE_DEVPROPKEY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8, pid) EXTERN_C const DEVPROPKEY name
#endif // INITGUID

// include DEVPKEY_Device_BusReportedDeviceDesc from WinDDK\7600.16385.1\inc\api\devpkey.h
DEFINE_DEVPROPKEY(DEVPKEY_Device_BusReportedDeviceDesc, 0x540b947e, 0x8b40, 0x45bc, 0xa8, 0xa2, 0x6a, 0x0b, 0x89, 0x4c, 0xbd, 0xa2, 4);     // DEVPROP_TYPE_STRING
DEFINE_DEVPROPKEY(DEVPKEY_Device_ContainerId, 0x8c7ed206, 0x3f8a, 0x4827, 0xb3, 0xab, 0xae, 0x9e, 0x1f, 0xae, 0xfc, 0x6c, 2);     // DEVPROP_TYPE_GUID
DEFINE_DEVPROPKEY(DEVPKEY_Device_FriendlyName, 0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 14);    // DEVPROP_TYPE_STRING
DEFINE_DEVPROPKEY(DEVPKEY_DeviceDisplay_Category, 0x78c34fc8, 0x104a, 0x4aca, 0x9e, 0xa4, 0x52, 0x4d, 0x52, 0x99, 0x6e, 0x57, 0x5a);  // DEVPROP_TYPE_STRING_LIST
DEFINE_DEVPROPKEY(DEVPKEY_Device_LocationInfo, 0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 15);    // DEVPROP_TYPE_STRING
DEFINE_DEVPROPKEY(DEVPKEY_Device_Manufacturer, 0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 13);    // DEVPROP_TYPE_STRING
DEFINE_DEVPROPKEY(DEVPKEY_Device_SecuritySDS, 0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 26);    // DEVPROP_TYPE_SECURITY_DESCRIPTOR_STRING

#define ARRAY_SIZE(arr)     (sizeof(arr)/sizeof(arr[0]))

#pragma comment (lib, "setupapi.lib")

typedef BOOL(WINAPI *FN_SetupDiGetDevicePropertyW)(
	__in       HDEVINFO DeviceInfoSet,
	__in       PSP_DEVINFO_DATA DeviceInfoData,
	__in       const DEVPROPKEY *PropertyKey,
	__out      DEVPROPTYPE *PropertyType,
	__out_opt  PBYTE PropertyBuffer,
	__in       DWORD PropertyBufferSize,
	__out_opt  PDWORD RequiredSize,
	__in       DWORD Flags
	);
// List all USB devices with some additional information
std::vector<USBDevice> USBUtils::GetUsbList()
{
	std::vector<USBDevice> usb = ListDevices(NULL, TEXT("USB"));
	std::vector<USBDevice> usbstor = ListDevices(NULL, TEXT("USBSTOR"));
	for (auto it = usbstor.begin(); it != usbstor.end(); it++)
	{
		usb.push_back(*it);
	}
	return usb;
}
std::map<stdstr, USBDevice> USBUtils::GetUsbInstanceMap()
{
	std::map<stdstr, USBDevice> usb;
	auto vec = GetUsbList();
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		bool bAppend = true;
		for (auto mit = usb.begin(); mit != usb.end(); mit++)
		{
			if (mit->second.DeviceInstanceID == it->DeviceInstanceID)
			{
				bAppend = false;
				break;
			}
			if (mit->second.pid.empty() && mit->second.vid.empty())
			{
				continue;
			}
			if (mit->second.pid == it->pid && it->vid == mit->second.vid)
			{
				bAppend = false;
				break;
			}	
		}
		if (bAppend)
		{
			usb.insert(std::make_pair(it->DeviceInstanceID, *it));
		}
	}
	return usb;
}
std::map<stdstr, USBDevice> USBUtils::GetUsbMap()
{
	std::map<stdstr, USBDevice> usb;
	auto vec = GetUsbList();
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		if (usb.find(it->DeviceInstanceID) == usb.end())
		{
			usb.insert(std::make_pair(it->DeviceInstanceID, *it));
		}
	}
	return usb;
}
std::vector<USBDevice> USBUtils::ListDevices(CONST GUID *pClassGuid, LPCTSTR pszEnumerator)
{
	std::vector<USBDevice> vec;
	unsigned i, j;
	DWORD dwSize, dwPropertyRegDataType;
	DEVPROPTYPE ulPropertyType;
	CONFIGRET status;
	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData;
	const static LPCTSTR arPrefix[3] = { TEXT("VID_"), TEXT("PID_"), TEXT("MI_") };
	TCHAR szDeviceInstanceID[MAX_DEVICE_ID_LEN];
	TCHAR szDesc[1024], szHardwareIDs[4096];
	WCHAR szBuffer[4096];
	LPTSTR pszToken, pszNextToken;
	TCHAR szVid[MAX_DEVICE_ID_LEN], szPid[MAX_DEVICE_ID_LEN], szMi[MAX_DEVICE_ID_LEN];
	FN_SetupDiGetDevicePropertyW fn_SetupDiGetDevicePropertyW = (FN_SetupDiGetDevicePropertyW)
		GetProcAddress(GetModuleHandle(TEXT("Setupapi.dll")), "SetupDiGetDevicePropertyW");
	if (!fn_SetupDiGetDevicePropertyW)
	{
		return vec;
	}
	// List all connected USB devices
	hDevInfo = SetupDiGetClassDevs(pClassGuid, pszEnumerator, NULL,
		pClassGuid != NULL ? DIGCF_PRESENT : DIGCF_ALLCLASSES | DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)
		return vec;

	// Find the ones that are driverless
	for (i = 0; ; i++)
	{
		DeviceInfoData.cbSize = sizeof(DeviceInfoData);
		if (!SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData))
			break;

		status = CM_Get_Device_ID(DeviceInfoData.DevInst, szDeviceInstanceID, MAX_PATH, 0);
		if (status != CR_SUCCESS)
			continue;
		USBDevice device;
		// Display device instance ID
		device.DeviceInstanceID = szDeviceInstanceID;

		if (SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_DEVICEDESC,
			&dwPropertyRegDataType, (BYTE*)szDesc,
			sizeof(szDesc),   // The size, in bytes
			&dwSize))
			device.Description = szDesc;

		if (SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_HARDWAREID,
			&dwPropertyRegDataType, (BYTE*)szHardwareIDs,
			sizeof(szHardwareIDs),    // The size, in bytes
			&dwSize))
		{
			LPCTSTR pszId;
			//_tprintf(TEXT("    Hardware IDs:\n"));
			for (pszId = szHardwareIDs;
				*pszId != TEXT('\0') && pszId + dwSize / sizeof(TCHAR) <= szHardwareIDs + ARRAYSIZE(szHardwareIDs);
				pszId += lstrlen(pszId) + 1)
			{
				//_tprintf(TEXT("        \"%s\"\n"), pszId);
			}
		}

		// Retreive the device description as reported by the device itself
		// On Vista and earlier, we can use only SPDRP_DEVICEDESC
		// On Windows 7, the information we want ("Bus reported device description") is
		// accessed through DEVPKEY_Device_BusReportedDeviceDesc
		if (fn_SetupDiGetDevicePropertyW && fn_SetupDiGetDevicePropertyW(hDevInfo, &DeviceInfoData, &DEVPKEY_Device_BusReportedDeviceDesc,
			&ulPropertyType, (BYTE*)szBuffer, sizeof(szBuffer), &dwSize, 0))
		{
			if (fn_SetupDiGetDevicePropertyW(hDevInfo, &DeviceInfoData, &DEVPKEY_Device_BusReportedDeviceDesc,
				&ulPropertyType, (BYTE*)szBuffer, sizeof(szBuffer), &dwSize, 0))
			{
				device.BusReportedDeviceDescription = szBuffer;
			}
			if (fn_SetupDiGetDevicePropertyW(hDevInfo, &DeviceInfoData, &DEVPKEY_Device_Manufacturer,
				&ulPropertyType, (BYTE*)szBuffer, sizeof(szBuffer), &dwSize, 0))
			{
				device.Manufacturer = szBuffer;
			}
			if (fn_SetupDiGetDevicePropertyW(hDevInfo, &DeviceInfoData, &DEVPKEY_Device_FriendlyName,
				&ulPropertyType, (BYTE*)szBuffer, sizeof(szBuffer), &dwSize, 0))
			{
				device.DeviceFriendlyName = szBuffer;
			}
			if (fn_SetupDiGetDevicePropertyW(hDevInfo, &DeviceInfoData, &DEVPKEY_Device_LocationInfo,
				&ulPropertyType, (BYTE*)szBuffer, sizeof(szBuffer), &dwSize, 0))
			{
				device.LocationInfo = szBuffer;
			}
			if (fn_SetupDiGetDevicePropertyW(hDevInfo, &DeviceInfoData, &DEVPKEY_Device_SecuritySDS,
				&ulPropertyType, (BYTE*)szBuffer, sizeof(szBuffer), &dwSize, 0))
			{
				// See Security Descriptor Definition Language on MSDN
				// (http://msdn.microsoft.com/en-us/library/windows/desktop/aa379567(v=vs.85).aspx)
				device.SecurityDescriptor = szBuffer;
			}
			if (fn_SetupDiGetDevicePropertyW(hDevInfo, &DeviceInfoData, &DEVPKEY_Device_ContainerId,
				&ulPropertyType, (BYTE*)szDesc, sizeof(szDesc), &dwSize, 0))
			{
				StringFromGUID2((REFGUID)szDesc, szBuffer, ARRAY_SIZE(szBuffer));
				device.ContainerId = szBuffer;
			}
			if (fn_SetupDiGetDevicePropertyW(hDevInfo, &DeviceInfoData, &DEVPKEY_DeviceDisplay_Category,
				&ulPropertyType, (BYTE*)szBuffer, sizeof(szBuffer), &dwSize, 0))
			{
				_tprintf(TEXT("    Device Display Category: \"%ls\"\n"), szBuffer);
				device.DisplayCategory = szBuffer;
			}

		}

		pszToken = _tcstok_s(szDeviceInstanceID, TEXT("\\#&"), &pszNextToken);
		while (pszToken != NULL)
		{
			szVid[0] = TEXT('\0');
			szPid[0] = TEXT('\0');
			szMi[0] = TEXT('\0');
			for (j = 0; j < 3; j++)
			{
				if (_tcsncmp(pszToken, arPrefix[j], lstrlen(arPrefix[j])) == 0)
				{
					switch (j)
					{
					case 0:
						_tcscpy_s(szVid, ARRAY_SIZE(szVid), pszToken);
						break;
					case 1:
						_tcscpy_s(szPid, ARRAY_SIZE(szPid), pszToken);
						break;
					case 2:
						_tcscpy_s(szMi, ARRAY_SIZE(szMi), pszToken);
						break;
					default:
						break;
					}
				}
			}
			if (szVid[0] != TEXT('\0'))
				device.vid = szVid;
			if (szPid[0] != TEXT('\0'))
				device.pid = szPid;
			if (szMi[0] != TEXT('\0'))
				device.mi = szMi;
			pszToken = _tcstok_s(NULL, TEXT("\\#&"), &pszNextToken);
		}
		vec.push_back(device);
	}

	return vec;
}