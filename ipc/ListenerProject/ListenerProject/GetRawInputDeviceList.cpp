#include <windows.h>
#include <iostream>
#include<string>
using namespace std;
void main2()
{
	UINT nDevices;
	PRAWINPUTDEVICELIST pRawInputDeviceList;
	if (GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST)) != 0)
	{
	}
	pRawInputDeviceList = new RAWINPUTDEVICELIST[nDevices];
	memset(pRawInputDeviceList, 0, sizeof(RAWINPUTDEVICELIST) * nDevices);
	if (GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST)))
	{
	}
	for (int i = 0; i < nDevices; i++)
	{
		RAWINPUTDEVICELIST &data = pRawInputDeviceList[i];
		cout << "Type:" << to_string(data.dwType) << endl;
	}
	delete[] pRawInputDeviceList;
}