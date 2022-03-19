#pragma once
#include<Windows.h>
#include<SetupAPI.h>
#include<stdio.h>
#include<devguid.h>
#pragma comment(lib, "SetupAPI.lib")
int GetDriversList()
{
	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD i;

	//https://docs.microsoft.com/zh-cn/windows/win32/api/setupapi/nf-setupapi-setupdigetclassdevsw?redirectedfrom=MSDN

	
	const GUID  ClassGuid[] = 
	{
		GUID_DEVCLASS_1394,
		GUID_DEVCLASS_1394DEBUG,
		GUID_DEVCLASS_61883,
		GUID_DEVCLASS_ADAPTER,
		GUID_DEVCLASS_APMSUPPORT,
		GUID_DEVCLASS_AVC,
		GUID_DEVCLASS_BATTERY,
		GUID_DEVCLASS_BIOMETRIC,
		GUID_DEVCLASS_BLUETOOTH,
		GUID_DEVCLASS_CDROM,
		GUID_DEVCLASS_COMPUTER,
		GUID_DEVCLASS_DECODER,
		GUID_DEVCLASS_DISKDRIVE,
		GUID_DEVCLASS_DISPLAY,
		GUID_DEVCLASS_DOT4,
		GUID_DEVCLASS_DOT4PRINT,
		GUID_DEVCLASS_ENUM1394,
		GUID_DEVCLASS_FDC,
		GUID_DEVCLASS_FLOPPYDISK,
		GUID_DEVCLASS_GPS,
		GUID_DEVCLASS_HDC,
		GUID_DEVCLASS_HIDCLASS,
		GUID_DEVCLASS_IMAGE,
		GUID_DEVCLASS_INFINIBAND,
		GUID_DEVCLASS_INFRARED,
		GUID_DEVCLASS_KEYBOARD,
		GUID_DEVCLASS_LEGACYDRIVER,
		GUID_DEVCLASS_MEDIA,
		GUID_DEVCLASS_MEDIUM_CHANGER,
		GUID_DEVCLASS_MEMORY,
		GUID_DEVCLASS_MODEM,
		GUID_DEVCLASS_MONITOR,
		GUID_DEVCLASS_MOUSE,
		GUID_DEVCLASS_MTD,
		GUID_DEVCLASS_MULTIFUNCTION,
		GUID_DEVCLASS_MULTIPORTSERIAL,
		GUID_DEVCLASS_NET,
		GUID_DEVCLASS_NETCLIENT,
		GUID_DEVCLASS_NETSERVICE,
		GUID_DEVCLASS_NETTRANS,
		GUID_DEVCLASS_NODRIVER,
		GUID_DEVCLASS_PCMCIA,
		GUID_DEVCLASS_PNPPRINTERS,
		GUID_DEVCLASS_PORTS,
		GUID_DEVCLASS_PRINTER,
		GUID_DEVCLASS_PRINTERUPGRADE,
		GUID_DEVCLASS_PROCESSOR,
		GUID_DEVCLASS_SBP2,
		GUID_DEVCLASS_SCSIADAPTER,
		GUID_DEVCLASS_SECURITYACCELERATOR,
		GUID_DEVCLASS_SENSOR,
		GUID_DEVCLASS_SIDESHOW,
		GUID_DEVCLASS_SMARTCARDREADER,
		GUID_DEVCLASS_SOUND,
		GUID_DEVCLASS_SYSTEM,
		GUID_DEVCLASS_TAPEDRIVE,
		GUID_DEVCLASS_UNKNOWN,
		GUID_DEVCLASS_USB,
		GUID_DEVCLASS_VOLUME,
		GUID_DEVCLASS_VOLUMESNAPSHOT,
		GUID_DEVCLASS_WCEUSBS,
		GUID_DEVCLASS_WPD,
		GUID_DEVCLASS_EHSTORAGESILO,
		GUID_DEVCLASS_FIRMWARE,
		GUID_DEVCLASS_EXTENSION
	};
	

	int guid_count = sizeof(ClassGuid) / sizeof(GUID);

	for (int guid_i = 0; guid_i < guid_count; guid_i++)
	{
		hDevInfo = SetupDiGetClassDevs(
			&ClassGuid[guid_i],
			//NULL,
			NULL, // Enumerator
			NULL,
			DIGCF_PRESENT);
		//DIGCF_PRESENT | DIGCF_ALLCLASSES );


		if (hDevInfo == INVALID_HANDLE_VALUE)
		{
			return 1;
		}

		printf("============================= %x ========================================\n", ClassGuid[guid_i]);

		// Enumerate through all devices in Set.
		DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
		{
			DWORD DataT;
			LPTSTR buffer = NULL;
			DWORD buffersize = 0;

			while (!SetupDiGetDeviceRegistryProperty(hDevInfo,
				&DeviceInfoData,
				SPDRP_DEVICEDESC,
				&DataT,
				(PBYTE)buffer,
				buffersize,
				&buffersize))
			{
				if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				{
					if (buffer) 
						LocalFree(buffer);
					buffer = (LPTSTR)LocalAlloc(LPTR, buffersize * 2);
				}
				else
				{
					break;
				}
			}

			printf("%ls\n", buffer);

			if (buffer) LocalFree(buffer);
		}

		if (GetLastError() != NO_ERROR && GetLastError() != ERROR_NO_MORE_ITEMS)
		{
			return 1;
		}

		SetupDiDestroyDeviceInfoList(hDevInfo);
	}

	return 0;

}