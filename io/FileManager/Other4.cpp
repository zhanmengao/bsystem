#include<Windows.h>
#include<iostream>
#include<vector>
#include<wchar.h>
using namespace std;
void main4()
{
	DWORD dwLen = GetLogicalDriveStrings(0, NULL);	//��ȡϵͳ�ַ�������.
	WCHAR * pszDriver = new WCHAR[dwLen];				//����һ����Ӧ���ȵ�����.
	WCHAR * pStr = pszDriver;
	GetLogicalDriveStrings(dwLen, pszDriver);		//��ȡ�̷��ַ���.
	vector<wstring> v;
	while (*pStr != '\0')
	{
		wcout << pStr << " ---- " << endl;
		v.push_back(pStr);
		pStr += lstrlen(pStr) + 1;			//��λ����һ���ַ���.��һ��Ϊ������'\0'�ַ���.
	}
	if (pszDriver != nullptr)
	{
		delete[] pszDriver;
		pszDriver = nullptr;
		pStr = nullptr;
	}
	int DType;
	int si = 0;
	BOOL fResult;
	unsigned _int64 i64FreeBytesToCaller;
	unsigned _int64 i64TotalBytes;
	unsigned _int64 i64FreeBytes;

	for (int i = 0; i < v.size(); ++i)
		//Ϊ����ʾÿ����������״̬����ͨ��ѭ�����ʵ�֣�����DStr�ڲ������������A:\NULLB:\NULLC:\NULL����������Ϣ������DSLength/4���Ի�þ����ѭ����Χ
	{
		wcout << v[i].c_str() << ":";
		DType = GetDriveType(v[i].c_str());
		//GetDriveType���������Ի�ȡ���������ͣ�����Ϊ�������ĸ�Ŀ¼
		if (DType == DRIVE_FIXED)
		{
			cout << "Ӳ��";
		}
		else if (DType == DRIVE_CDROM)
		{
			cout << "����";
		}
		else if (DType == DRIVE_REMOVABLE)
		{
			cout << "���ƶ�ʽ����";
		}
		else if (DType == DRIVE_REMOTE)
		{
			cout << "�������";
		}
		else if (DType == DRIVE_RAMDISK)
		{
			cout << "����RAM����";
		}
		else if (DType == DRIVE_UNKNOWN)
		{
			cout << "δ֪�豸";
		}

		fResult = GetDiskFreeSpaceEx(v[i].c_str(),
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);
		//GetDiskFreeSpaceEx���������Ի�ȡ���������̵Ŀռ�״̬,�������ص��Ǹ�BOOL��������
		//ͨ�����ص�BOOL�����ж��������Ƿ��ڹ���״̬
		if (fResult)
		{
			cout << " �ܴ�С:" << (float)i64TotalBytes / 1024 / 1024 << " MB";//����������
			cout << " ʣ���С:" << (float)i64FreeBytesToCaller / 1024 / 1024 << " MB";//����ʣ��ռ�
		}
		else
		{
			cout << " �豸δ׼����";
		}
		cout << endl;
		si += 4;
	}

}