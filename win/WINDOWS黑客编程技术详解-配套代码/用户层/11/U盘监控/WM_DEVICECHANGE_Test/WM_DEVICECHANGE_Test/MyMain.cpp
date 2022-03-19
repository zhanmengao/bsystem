#include "resource.h"

#include <Windows.h>
#include <Dbt.h>


LRESULT OnDeviceChange(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	// �豸�Ѿ�����
	case DBT_DEVICEARRIVAL:               
	{
		PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
		// �߼���
		if (DBT_DEVTYP_VOLUME == lpdb->dbch_devicetype)  
		{
			// ���� dbcv_unitmask ������豸�̷�
			PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
			DWORD dwDriverMask = lpdbv->dbcv_unitmask;
			DWORD dwTemp = 1;
			char szDriver[4] = "A:\\";
			for (szDriver[0] = 'A'; szDriver[0] <= 'Z'; szDriver[0]++)
			{
				if (0 < (dwTemp & dwDriverMask))
				{
					// ��ȡ�豸�̷�
					::MessageBox(NULL, szDriver, "�豸�Ѳ���", MB_OK);
				}
				// ����1λ, �����ж���һ���̷�
				dwTemp = (dwTemp << 1);   
			}
		}
		break;
	}
	// �豸�Ѿ��Ƴ�
	case DBT_DEVICEREMOVECOMPLETE:        
	{
		PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
		// �߼���
		if (DBT_DEVTYP_VOLUME == lpdb->dbch_devicetype)
		{
			// ���� dbcv_unitmask ������豸�̷�
			PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
			DWORD dwDriverMask = lpdbv->dbcv_unitmask;
			DWORD dwTemp = 1;
			char szDriver[4] = "A:\\";
			for (szDriver[0] = 'A'; szDriver[0] <= 'Z'; szDriver[0]++)
			{
				if (0 < (dwTemp & dwDriverMask))
				{
					// ��ȡ�豸�̷�
					::MessageBox(NULL, szDriver, "�豸���Ƴ�", MB_OK);
				}
				// ����1λ, �����ж���һ���̷�
				dwTemp = (dwTemp << 1);
			}
		}
		break;
	}
	default:
		break;
	}

	return 0;
}


BOOL CALLBACK ProgMainDlg(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_DEVICECHANGE == uiMsg)
	{
		OnDeviceChange(wParam, lParam);
	}
	else if (WM_CLOSE == uiMsg)
	{
		::EndDialog(hWnd, NULL);
	}

	return FALSE;
}


int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevinstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	::DialogBoxParam(hInstance, (LPCSTR)IDD_DIALOG1, NULL, (DLGPROC)ProgMainDlg, NULL);

	::ExitProcess(NULL);
	return 0;
}



