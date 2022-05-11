#pragma once
#include<Windows.h>
#include<atlstr.h>


#define PRINT_LEVEL 2
void ShowPrinterStatus(TCHAR* PrinterName);
void GetPrinterList()
{
	DWORD dwSize, dwPrinters;
	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, PRINT_LEVEL, NULL, 0, &dwSize, &dwPrinters);
	BYTE* pBuffer = new BYTE[dwSize];
	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, PRINT_LEVEL, pBuffer, dwSize, &dwSize, &dwPrinters);
	if (dwPrinters != 0)
	{
		PRINTER_INFO_2* pPrnInfo = (PRINTER_INFO_2*)pBuffer;
		for (UINT i = 0; i<dwPrinters; i++)
		{
			if (pPrnInfo->Attributes & PRINTER_ATTRIBUTE_LOCAL)
			{
				printf("%ws : %lu %.2x %d\n", pPrnInfo->pPrinterName, pPrnInfo->Attributes, pPrnInfo->Status);
				//PrintfBinary(pPrnInfo->Attributes);
				DWORD data = pPrnInfo->Attributes & (DWORD)PRINTER_ATTRIBUTE_WORK_OFFLINE;
				if (data > 0)
				{
					printf("Offline \n");
				}
				else
				{
					printf("Online \n");
				}
				ShowPrinterStatus(pPrnInfo->pPrinterName);
			}
			pPrnInfo++;
		}
	}
	delete[] pBuffer;
}
int GetPrintJobList(CString szPrintName)
{
	HANDLE hPrinter;
	DWORD dwNeeded, dwReturned, i;
	JOB_INFO_1* pJobInfo;
	DWORD res = 0;
	res = OpenPrinter((LPWSTR)(LPCWSTR)szPrintName, &hPrinter, NULL);
	if (!res)
		return -1;
	res = EnumJobs(hPrinter, 0, 0xFFFFFFFF, 1, 0, 0, &dwNeeded, &dwReturned);
	if (res)
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			//��ӡ�б���û����;
			ClosePrinter(hPrinter);
			return 0;
		}
	}
	pJobInfo = (JOB_INFO_1*)malloc(dwNeeded);
	if (pJobInfo == NULL)
	{
		ClosePrinter(hPrinter);
		return -1;
	}

	ZeroMemory(pJobInfo, dwNeeded);
	DWORD dwByteUsed;
	res = EnumJobs(hPrinter, 0, 0xFFFFFFFF, 1, (LPBYTE)pJobInfo, dwNeeded, &dwByteUsed, &dwReturned);
	if (!res)
	{
		//MessageBox("print 4");
		ClosePrinter(hPrinter);
		free(pJobInfo);
		return -1;
	}
	res = pJobInfo[0].Status;
	free(pJobInfo);
	ClosePrinter(hPrinter);
	return res;
}


void ShowPrinterStatus(TCHAR* PrinterName)
{
	HANDLE printerHandle;                //��ӡ���豸���

										 //���򿪴�ӡ���豸�Ƿ�ɹ�
	if (!OpenPrinter(PrinterName, &printerHandle, NULL))
		return;

	DWORD nByteNeeded;
	DWORD nReturned;
	DWORD nByteUsed;

	//ͨ������GetPrinter()�����õ���ҵ����
	PRINTER_INFO_2* pPrinterInfo = NULL;
	GetPrinter(printerHandle, 2, NULL, 0, &nByteNeeded);
	pPrinterInfo = (PRINTER_INFO_2*)malloc(nByteNeeded);
	GetPrinter(printerHandle, 2, (LPBYTE)pPrinterInfo, nByteNeeded, &nByteUsed);

	//ͨ������EnumJobs()����ö�ٴ�ӡ����
	JOB_INFO_2* pJobInfo = NULL;
	EnumJobs(printerHandle, 0, pPrinterInfo->cJobs, 2, NULL, 0,
		(LPDWORD)&nByteNeeded, (LPDWORD)&nReturned);
	pJobInfo = (JOB_INFO_2*)malloc(nByteNeeded);
	ZeroMemory(pJobInfo, nByteNeeded);
	EnumJobs(printerHandle, 0, pPrinterInfo->cJobs, 2, (LPBYTE)pJobInfo, nByteNeeded,
		(LPDWORD)&nByteUsed, (LPDWORD)&nReturned);

	//��⵱ǰ�Ƿ��д�ӡ����
	if (pPrinterInfo->cJobs != 0)
	{
		//ֽ������
		CString strPageSize = _T("");
		if (pJobInfo[0].pDevMode->dmPaperSize == DMPAPER_A4)
			strPageSize = _T("A4");
		else if (pJobInfo[0].pDevMode->dmPaperSize == DMPAPER_B5)
			strPageSize = _T("B5");

		//��ӡ����
		CString strPrintCopies = _T("");
		strPrintCopies.Format(_T("%d"), pJobInfo[0].pDevMode->dmCopies);

		//��ӡ��ɫ
		CString strPrintColor = _T("");
		if (pJobInfo[0].pDevMode->dmColor == DMCOLOR_COLOR)
			strPrintColor = _T("��ɫ");
		else if (pJobInfo[0].pDevMode->dmColor == DMCOLOR_MONOCHROME)
			strPrintColor = _T("�ڰ�");

		//��ӡʱ��
		CString strSubmitted = _T("");
		strSubmitted.Format(_T("%d-%d-%d %d:%d:%d"),
			pJobInfo[0].Submitted.wYear, pJobInfo[0].Submitted.wMonth, pJobInfo[0].Submitted.wDay,
			pJobInfo[0].Submitted.wHour + 8, pJobInfo[0].Submitted.wMinute, pJobInfo[0].Submitted.wSecond);
	}
	free(pPrinterInfo);
	free(pJobInfo);
	//�رմ�ӡ���豸
	ClosePrinter(printerHandle);
}

