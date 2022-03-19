#include<Windows.h>
#include<stdio.h>
#include<tchar.h>

//#include<d3d9caps.h>
//#include<d3d9.h>
//#include<d3d9helper.h>

//void mainDis()
//{
//	D3DCAPS9 caps;
//	DEVICE->GetDeviceCaps(&caps);
//	D3DDISPLAYMODE dismode;
//	D3D->GetAdapterDisplayMode(caps.AdapterOrdinal, &dismode);
//	int count = D3D->GetAdapterModeCount(caps.AdapterOrdinal, dismode.Format);
//	for (int i = 0; i < count; ++i)
//	{
//		EnumAdapterModes(caps.AdapterOrdinal, dismode.Format, i, &dismode);
//		//dismode.Width;
//		//dismode.Height;
//}
void main323123()
{
	DISPLAY_DEVICE dd;
	memset(&dd, 0, sizeof(DISPLAY_DEVICE));
	dd.cb = sizeof(dd);
	int i = 0;
	while (EnumDisplayDevices(NULL, i, &dd, 0))
	{
		wprintf(_T("Device Name: %s Device String: %s \n"), dd.DeviceName, dd.DeviceString);

		if (EnumDisplayDevices(dd.DeviceName, 0, &dd, 0))
		{
			wprintf(_T("Monitor Name: %s Monitor String: %s \n"), dd.DeviceName, dd.DeviceString);
		}

		i++;
	}
}