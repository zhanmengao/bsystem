#include"CINI.h"
#include"INIManager.hpp"
#include<atlstr.h>
#include"AIDAmain.h"

using namespace std;

struct A
{
	int n;
	char ch;
};
enum DISK_TYPE
{
	DISK_INVALID,
	DISK_ATA,
	DISK_NVME,
};

#define INI_PATH _T("D:\\Space\\System\\Setting\\Readini\\Report2.ini")
void mainTest() {

	TCHAR temp[20] = { 0 };
	TCHAR name[20] = { 0 };
	A a;
	a.n = 600;
	a.ch = 'a';

	wsprintf(temp, _T("%d"), 100);
	// �������ļ���д���ַ���
	WritePrivateProfileString(_T("Device"), _T("Name"), _T("PC"), INI_PATH);
	// �������ļ���д������������Ҫ������ת�����ַ���
	WritePrivateProfileString(_T("Device"), _T("Count"), temp, INI_PATH);
	// д��ṹ��
	WritePrivateProfileStruct(_T("Device"), _T("Struct"), &a, sizeof(a), INI_PATH);

	// ��ȡ�����ļ��е��ַ���
	GetPrivateProfileString(_T("Device"), _T("Name"), _T(""), name, sizeof(name), INI_PATH);
	// ��ȡһ������ֵ
	int count = GetPrivateProfileInt(_T("Device"), _T("Count"), 0, INI_PATH);
	// ��ȡ�ṹ��
	A aa = { 0 };
	GetPrivateProfileStruct(_T("Device"), _T("Struct"), &aa, sizeof(aa), INI_PATH);


	STDOUT << L"Name:" << stdstr(name).c_str() << endl;
	cout << "Count:" << count << endl;
	cout << "Struct.n:" << aa.n << endl;
	cout << "Struct.ch:" << aa.ch << endl;
	cout << endl;
}

void maininiManager()
{
	iniManager im(INI_PATH);
	STDOUT << _T("Name:") << im.ReadString(_T("Device"), _T("Name")) << endl;
	cout << "nun:" << im.ReadNum(_T("Device"), _T("Count")) << endl;
}

void main3()
{
	TCHAR temp[10240] = { 0 };
	GetPrivateProfileSection(_T("ATA"), temp, sizeof(temp), INI_PATH);
	cout << temp << endl;

	iniManager ini;
	ini.SetPath(INI_PATH);
	std::vector<INI_KV> val;
	ini.ReadSection(_T("ATA"), val);
	for (auto it = val.begin(); it != val.end(); it++)
	{
		STDOUT <<_T("Key	")<< it->Key <<_T("	Val	")<< it->Val << endl;
	}

	GetPrivateProfileSectionNames(temp, sizeof(temp), INI_PATH);
	std::vector<stdstr> vec;
	ini.ReadSectionNames(vec);

	//veidp
	stdstr SerialNumber;
	for (int j = 0; j < 10; j++)
	{
		DISK_TYPE type = DISK_INVALID;
		TCHAR keyName[1024] = { 0 };
		wsprintf(keyName, _T("ATA%d|ATA �豸��Ϣ|���к�"), j + 1);
		SerialNumber = ini.ReadString(_T("ATA"), keyName, _T(""));
		//�п��ܲ���ATA
		if (!SerialNumber.empty())
		{
			type = DISK_ATA;
		}
		else
		{
			wsprintf(keyName, _T("ATA%d|NVMe �豸��Ϣ|���к�"), j + 1);
			SerialNumber = ini.ReadString(_T("ATA"), keyName, _T(""));
			if (!SerialNumber.empty())
			{
				type = DISK_NVME;
			}
		}
		switch (type)
		{
		case DISK_ATA:
		{
			wsprintf(keyName, _T("ATA%d|ATA �豸��Ϣ|�豸����"), j + 1);
			stdstr InterfaceType = ini.ReadString(_T("ATA"), keyName, _T(""));
			wsprintf(keyName, _T("ATA%d|ATA �豸��Ϣ|��Ƭת��"), j + 1);
			stdstr rev = ini.ReadString(_T("ATA"), keyName, _T("*"));
		}
		break;
		case DISK_NVME:
		{
			wsprintf(keyName, _T("ATA%d|NVMe �豸��Ϣ|�豸����"), j + 1);
			stdstr InterfaceType = ini.ReadString(_T("ATA"), keyName, _T(""));
			wsprintf(keyName, _T("ATA%d|NVMe �豸��Ϣ|��Ƭת��"), j + 1);
			stdstr rev = ini.ReadString(_T("ATA"), keyName, _T("*"));
		}
		break;
		}
		break;
	}
	system("pause");
}

int main()
{
	AidaMain();
}