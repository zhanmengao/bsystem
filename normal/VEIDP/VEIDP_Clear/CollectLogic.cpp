#include"stdafx.h"
#include"CollectRoutinue.h"
#include"../../SystemInfo/SystemInfo/SysInfoUtils.h"
#include"../../../../Network/SocketEncode/SocketEncode/base64.h"
#include"../../../Setting/WDesktop/LnkUtils.h"
#include<direct.h>
#include"../../../File/FileManager/FileManager/DiskManager.h"
#include <windows.h>
#include <urlmon.h>
#include <string>
#include <vector>
#include <algorithm>
#include"../../../Process/WindowsProcessHelper/Fork/ForkUtils.h"
#include"../../SystemInfo/HardwareUtils/MonitUtils.h"
#pragma comment(lib, "urlmon.lib")
#include "Wincrypt.h"
enum DISK_TYPE
{
	DISK_INVALID,
	DISK_ATA,
	DISK_NVME,
};
BOOL CollectRoutinue::GetIDPDisk(std::vector<IDPDrive>& vecDrive, std::vector<IDPDisk>& vecDisk)
{
	//Windows API
	auto diskVec = DiskTools::GetDiskInfo();
	for (auto it = diskVec.begin(); it != diskVec.end(); it++)
	{
		IDPDisk disk;
		disk.totalSize = it->TotalSize / 1024 / 1024 / 1024;
		disk.residueSize = it->freeSize / 1024 / 1024 / 1024;
		disk.name = it->diskName;
		vecDisk.push_back(disk);
	}

	vector<WMIDisk> wmivec;
	std::string exce;
	if (mWMI.Begin() && mWMI.GetDiskInfo(wmivec, exce))
	{
		for (auto it = wmivec.begin(); it != wmivec.end(); it++)
		{
			IDPDrive drive;
			drive.brand = it->csModel;
			drive.SerialNumber = it->SerialNumber;
			drive.totalSize = it->csSize / 1024;
			drive.InterfaceType = it->csInterfaceType;
			drive.rev = "*";
			vecDrive.push_back(drive);
		}
		mWMI.End();
	}
	else
	{
		LOG(std::string("WMI GetDiskInfo Error") + ErrorUtils::ErrorMessage(mWMI.GetLastErrorWMI()) + exce);
	}


	return TRUE;
}
BOOL CollectRoutinue::GetIDPMemory(std::vector<IDPMemory>& vec)
{
	vector<WMIMemory> wmivec;
	std::string exce;
	if (mWMI.Begin() && mWMI.GetMemorySlots(wmivec, exce))
	{
		for (auto it = wmivec.begin(); it != wmivec.end(); it++)
		{
			IDPMemory info;
			info.SerialNumber = it->csSerialNum;
			info.size = it->csCapacity / 1024 / 1024 / 1024 + " G";
			info.brand = it->Manufacturer;
			info.memoryMZ = it->mSpeed + " MT/s";
			info.series = it->csType;
			vec.push_back(info);
		}
		mWMI.End();
	}
	else
	{
		LOG(std::string("WMI GetMemorySlots Error	") + ErrorUtils::ErrorMessage(mWMI.GetLastErrorWMI()) +
			"	" + exce);
	}
	return TRUE;
}
BOOL CollectRoutinue::GetPrinter(std::vector<IDPPrinter>& vec)
{
	vector<WMIPrint> wmivec;
	std::string error;
	if (mWMI.Begin() && mWMI.GetPrinterInfo(wmivec, error))
	{
		for (int i = 0; i < wmivec.size(); i++)
		{
			auto& info = wmivec[i];
			IDPPrinter data;
			data.Name = info.csName;
			if (info.Default)
			{
				data.Default = "��";
			}
			else
			{
				data.Default = "��";
			}
			data.Driver = info.csDeviceName;

			if (info.Shared)
			{
				data.Share = "����";
			}
			else
			{
				data.Share = "δ����";
			}
			data.Status = info.Status;
			vec.push_back(data);
		}
		mWMI.End();
	}
	else
	{
		LOG(std::string("WMI GetPrinterInfo Error") + ErrorUtils::ErrorMessage(mWMI.GetLastErrorWMI()) + "	" + error);
	}
	return TRUE;
}
BOOL CollectRoutinue::GetIDPCPU(std::vector<IDPCPU>& vec)
{
	//ini
	IDPCPU _cpu;
	//ֱ�Ӵӻ���ȡ
	_cpu.cpuid = CPUID;
	std::string error;
	vector<WMICPU> wmivec;
	if (mWMI.Begin() && mWMI.GetCPU(wmivec, error))
	{
		for (auto it = wmivec.begin(); it != wmivec.end(); it++)
		{
			if (IsInvalidAIDAValue(_cpu.brand))
			{
				_cpu.brand = it->Manufacturer;
			}
			if (IsInvalidAIDAValue(_cpu.series))
			{
				_cpu.series = it->Name;
			}
			if (IsInvalidAIDAValue(_cpu.Tjmax))
			{
				_cpu.Tjmax = it->ExtClock + " ��C";
			}
		}
	}
	else
	{
		LOG("mWMI.GetCPU Error" + ErrorUtils::ComErrorMessage(mWMI.GetLastErrorWMI()) + "	" + error);
	}
	vec.push_back(_cpu);
	return TRUE;
}

BOOL CollectRoutinue::GetIDPMother(std::vector<IDPMother>& vec)
{
	if (mWMI.Begin())
	{
		vector<WMIMainBoard> wmivec;
		std::string exce;
		if (mWMI.GetMainBoard(wmivec, exce))
		{
			for (auto it = wmivec.begin(); it != wmivec.end(); it++)
			{
				IDPMother mother;
				mother.name = it->Name;
				mother.brand = it->Manufacturer;
				vec.push_back(mother);
			}
		}
		else
		{
			LOG(std::string("WMI GetMainBoard Error") + ErrorUtils::ErrorMessage(mWMI.GetLastErrorWMI()) + exce);
		}
		mWMI.End();
	}
	return TRUE;
}

BOOL CollectRoutinue::GetKeyboard(std::vector<IDPKeyboard>& vec)
{
	if (mWMI.Begin())
	{
		vector<WMIKeyboard> wmivec;
		std::string exce;
		if (mWMI.GetKeyBoardInfo(wmivec, exce))
		{
			for (auto it = wmivec.begin(); it != wmivec.end(); it++)
			{
				IDPKeyboard kb;
				kb.name = it->csDescription;
				kb.type = it->csCaption;

				vec.push_back(kb);
			}
		}
		else
		{
			LOG(std::string("WMI GetKeyboard Error") + ErrorUtils::ErrorMessage(mWMI.GetLastErrorWMI()) + exce);
		}
		mWMI.End();
	}
	return TRUE;
}

BOOL CollectRoutinue::GetMouse(std::vector<IDPMouse>& vec)
{
	if (mWMI.Begin())
	{
		vector<WMIPointer> wmivec;
		std::string exce;
		if (mWMI.GetPointerInfo(wmivec, exce))
		{
			for (auto it = wmivec.begin(); it != wmivec.end(); it++)
			{
				IDPMouse kb;
				kb.name = it->csDescription;

				vec.push_back(kb);
			}
		}
		else
		{
			LOG(std::string("WMI GetKeyboard Error") + ErrorUtils::ErrorMessage(mWMI.GetLastErrorWMI()) + exce);
		}
		mWMI.End();
	}
	return TRUE;
}
BOOL CollectRoutinue::GetMonit(std::vector<IDPMonit>& vec)
{
	//EDID�ӿ� ����
	std::vector<MonitInfo> apivec;
	MonitUtils::GetMonitInfo(apivec);
	//��EDID���AIDA�����кſ���Ϊ�գ�
	for (auto it = apivec.begin(); it != apivec.end(); it++)
	{
		IDPMonit data;
		data.brand = it->edid.csName;
		//���к�
		data.SerialNumber = it->edid.csSerialNum;
		//�ͺ�����
		data.model = it->edid.csDescription;
		//��������Ļ
		data.size = it->edid.MonitorSize();
		//���ֱ���
		data.imageSize = it->monitor.MaxMonitorMode().Desc();
		//ID
		data.ID = it->edid.csDescription;
		//������
		data.maker = it->edid.csManufacturer;
		vec.push_back(data);
	}
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		auto& info = *it;
		//������
		if (IsInvalidAIDAValue(info.model))
		{
			if (!IsInvalidAIDAValue(info.maker))
			{
				info.model = info.maker;
			}
			if (!IsInvalidAIDAValue(info.ID))
			{
				if (IsInvalidAIDAValue(info.model))
				{
					info.model = info.ID;
				}
				else
				{
					info.model += " " + info.ID;
				}
			}
		}
	}
	return TRUE;
}
bool CollectRoutinue::ReadPhotoFile(std::basic_string<TCHAR> strFileName, std::string &strData)
{
	HANDLE hFile;
	hFile = CreateFile(strFileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dFileSize = GetFileSize(hFile, NULL);
	char * pBuffer = new char[dFileSize + 1];

	if (pBuffer == NULL)
		return false;

	memset(pBuffer, 0, dFileSize);

	DWORD dReadSize(0);
	if (!ReadFile(hFile, pBuffer, dFileSize, &dReadSize, NULL))
	{
		LOG("ReadFile Error" + strFileName);
		delete[]pBuffer;
		CloseHandle(hFile);
		return false;
	}

	CBase64 base64;
	strData = "";
	if (!base64.Encode((const char*)pBuffer, dReadSize, strData))
	{
		LOG("base64.Encode Error");
	}

	delete[] pBuffer;
	CloseHandle(hFile);
	return true;
}

int CollectRoutinue::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;
	UINT  size = 0;
	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return -1;
}

BOOL CollectRoutinue::SetImaeTextDate(string filename1)
{

	time_t now = time(0);
	tm *ltm = localtime(&now);
	/**
	* ��ͼƬ�ϼ�����
	* ============================================================================================== */

	//https://docs.microsoft.com/zh-cn/windows/win32/api/gdiplusheaders/nl-gdiplusheaders-image
	//https://docs.microsoft.com/zh-cn/windows/win32/api/gdiplusgraphics/nl-gdiplusgraphics-graphics
	//

	stdstr filename0;
	filename0 = filename1.c_str();

	std::wstring wOldFileName = Convert::StringToWString(filename0);								//ԭ�ļ���

	Gdiplus::Image image2(wOldFileName.c_str());
	Gdiplus::Graphics imageGraphics(&image2);
	imageGraphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

	CString wxText;
	char buffer2[MAX_PATH] = { 0 };
	DWORD code = GetEnvironmentVariable("OS", buffer2, MAX_PATH);
	//sprintf(buffer2, "%s\\Screen.jpg", buffer1);

	wxText.Format(_TEXT("%d �� %.2d �� %.2d �� %.2d:%.2d:%.2d\n%s"),
		ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec, buffer2);

	Gdiplus::FontFamily fontFamily(L"Microsoft YaHei");
	Gdiplus::Font myFont(&fontFamily, 20, Gdiplus::FontStyleRegular, Gdiplus::UnitPoint);
	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));
	Gdiplus::PointF school_site((Gdiplus::REAL)10, (Gdiplus::REAL)10);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentNear);

	auto wx = wxText.GetBuffer();
	auto strWrite = Convert::StringToWString(wx);									//Ҫд����
	wxText.ReleaseBuffer();
	imageGraphics.DrawString(strWrite.c_str(), strWrite.size(), &myFont, school_site, &format, &blackBrush);

	CString newfilename = wOldFileName.c_str();							//���ļ���
	newfilename.Replace(".jpg", "_TEXT.jpg");
	CLSID pngClsid;
	GetEncoderClsid(L"image/jpeg", &pngClsid);

	wx = newfilename.GetBuffer();
	std::wstring wNewFileName = Convert::StringToWString(wx);
	newfilename.ReleaseBuffer();

	auto status = image2.Save(wNewFileName.c_str(), &pngClsid, NULL);
	if (status == Gdiplus::Ok)
	{
		LOG_DAILY("Save JPEG OK Path : " + Convert::WStringToString(wNewFileName));
		return TRUE;
	}
	else
	{
		LOG("Save JPEG Error Status : " + to_string(status));
		return FALSE;
	}
	return 0;
}
int CollectRoutinue::GetWindowScreen2(const stdstr& path)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);


	//https://docs.microsoft.com/zh-cn/windows/win32/api/wingdi/
	//https://docs.microsoft.com/en-us/cpp/atl-mfc-shared/reference/cimage-class?view=vs-2015
	//https://docs.microsoft.com/zh-cn/windows/win32/api/wingdi/nf-wingdi-bitblt	

	HDC hdcSrc = ::GetDC(NULL);
	int nBitPerPixel = GetDeviceCaps(hdcSrc, BITSPIXEL);
	int nWidth = GetDeviceCaps(hdcSrc, HORZRES);
	int nHeight = GetDeviceCaps(hdcSrc, VERTRES);

	CImage image;
	image.Create(nWidth, nHeight, nBitPerPixel);

	BitBlt(image.GetDC(), 0, 0, nWidth, nHeight, hdcSrc, 0, 0, SRCCOPY);
	::ReleaseDC(NULL, hdcSrc);

	HRESULT saveJPEG = image.Save(path.c_str(), Gdiplus::ImageFormatJPEG);

	image.ReleaseDC();
	image.Destroy();

	if (S_OK == saveJPEG)
	{
		//ͼƬ�ϼ����ִ�
		if (SetImaeTextDate(path))
		{
			return 0;
		}
	}
	else
	{
		DWORD error = GetLastError();
		LOG(string("Save Normal JPEG Error ") + path + "  HRESULT : " + to_string(saveJPEG));
		return error;
	}
	return 0;
}

vector<string> split(const string &s, const string &seperator)
{
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;


	/*
	http://192.168.50.35:8080/DataMapManager/veTerminal/veidp.exe
	vector<string> v = split(data_downloadAdd, "/");
	*/
	while (i != s.size())
	{
		//�ҵ��ַ������׸������ڷָ�������ĸ��
		int flag = 0;
		while (i != s.size() && flag == 0)
		{
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x])
				{
					++i;
					flag = 0;
					break;
				}
		}

		//�ҵ���һ���ָ������������ָ���֮����ַ���ȡ����
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0)
		{
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x])
				{
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j)
		{
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}

/***
* ȡ��������
*/
std::string Get_ENV(char* hzName)
{

	char* hzName_v;
	size_t requiredSize;

	getenv_s(&requiredSize, NULL, 0, hzName);
	if (requiredSize == 0)
	{
		return NULL;
	}

	hzName_v = (char*)malloc(requiredSize * sizeof(wchar_t));
	if (!hzName_v)
	{
		return NULL;
	}
	getenv_s(&requiredSize, hzName_v, requiredSize, hzName);
	std::string ret;
	if (hzName_v)
	{
		ret = hzName_v;
		free(hzName_v);
		hzName_v = NULL;
	}
	return ret;

}
string GetMd5(LPCSTR FileDirectory)
{
	HANDLE hFile = CreateFile(FileDirectory, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)																//���CreateFile����ʧ��
	{
		cout << "CreateFile go wrong :" << GetLastError() << endl;									//��ʾCreateFile����ʧ�ܣ����������š�visual studio�п��ڡ����ߡ�>��������ҡ������ô���ŵõ�������Ϣ��
		CloseHandle(hFile);
		return "";
	}
	HCRYPTPROV hProv = NULL;
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == FALSE)       //���CSP��һ����Կ�����ľ��
	{
		cout << "CryptAcquireContext go wrong:" << GetLastError() << endl;
	}
	HCRYPTPROV hHash = NULL;
	if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash) == FALSE)									//��ʼ������������hash������������һ����CSP��hash������صľ��������������������CryptHashData���á�
	{
		cout << "CryptCreateHash go wrong:" << GetLastError() << endl;
	}
	DWORD dwFileSize = GetFileSize(hFile, 0);		//��ȡ�ļ��Ĵ�С
	if (dwFileSize == 0xFFFFFFFF)					//�����ȡ�ļ���Сʧ��
	{
		cout << "GetFileSize go wrong:" << GetLastError() << endl;
	}

	BYTE* lpReadFileBuffer = new BYTE[dwFileSize];
	DWORD lpReadNumberOfBytes;
	if (ReadFile(hFile, lpReadFileBuffer, dwFileSize, &lpReadNumberOfBytes, NULL) == 0)			//��ȡ�ļ�
	{
		cout << "ReadFile go wrong:" << GetLastError() << endl;
	}
	if (CryptHashData(hHash, lpReadFileBuffer, lpReadNumberOfBytes, 0) == FALSE)				//hash�ļ�
	{
		cout << "CryptHashData go wrong:" << GetLastError() << endl;
	}

	delete[] lpReadFileBuffer;
	CloseHandle(hFile);								//�ر��ļ����
	BYTE *pbHash;
	DWORD dwHashLen = sizeof(DWORD);

	if (CryptGetHashParam(hHash, HP_HASHVAL, NULL, &dwHashLen, 0))      //��Ҳ��֪��ΪʲôҪ����������CryptGetHashParam������ǲ��յ�msdn       
	{
	}
	else
	{
		cout << "get length wrong" << endl;
	}
	if (pbHash = (BYTE*)malloc(dwHashLen))
	{
	}
	else
	{
		cout << "allocation failed" << endl;
	}

	string md5;

	char firstNum[32] = { 0 };

	if (CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0))            //���md5ֵ
	{
		for (DWORD i = 0; i < dwHashLen; i++)         //���md5ֵ
		{
			sprintf_s(firstNum, "%02x", pbHash[i]);
			md5 += firstNum;
			//printf("%02x", pbHash[i]);
		}

		cout << endl;
	}

	//	printf("%s", md5);

	//�ƺ���
	if (pbHash)
	{
		free(pbHash);
		pbHash = NULL;
	}
	if (CryptDestroyHash(hHash) == FALSE)          //����hash����
	{
		cout << "CryptDestroyHash go wrong:" << GetLastError() << endl;
	}
	if (CryptReleaseContext(hProv, 0) == FALSE)
	{
		cout << "CryptReleaseContext go wrong:" << GetLastError() << endl;
	}
	return md5;
}
BOOL DownloadFiles(const CHAR* url, const CHAR* filename, string &hashCode)
{
	//����д�Ķ�ת����Сд
	transform(hashCode.begin(), hashCode.end(), hashCode.begin(), ::tolower);
	string md5 = "";
	//do{
	remove(filename);
	string s = filename;
	HRESULT hr = URLDownloadToFile(NULL, url, filename, 0, 0);
	if (hr == S_OK)
	{
		md5 = GetMd5(s.c_str());

		//����д�Ķ�ת����Сд
		transform(md5.begin(), md5.end(), md5.begin(), ::tolower);

		if (md5 == hashCode)
		{
			//MessageBox(0, hashCode.c_str(), "����ļ�", 1);
			return TRUE;
		}
		else
		{
			remove(filename);
		}
	}
	else
	{
		remove(filename);
	}
	//} while (md5.c_str() != hashCode.c_str());
	//MessageBox(0, hashCode.c_str(), "�ļ�", 1);
	return TRUE;
}

int HttpDownFile(const  string &strUrl, const  string &filename, string &hashCode)
{
	DownloadFiles(strUrl.c_str(), filename.c_str(), hashCode);
	return TRUE;
}
void CollectRoutinue::Download(int instruct, bool ifSortSent, const string&  strRet)
{
	Json::Reader reader;
	Json::Value  resp;

	if (reader.parse(strRet, resp, false))
	{
		string data_downloadAdd = "";
		string data_hashCode = "";

		Json::Value  r_data = resp["data"];

		data_downloadAdd = r_data["filename"].asString();
		data_hashCode = r_data["md5"].asString();
		string type = r_data["updateType"].asString();

		if (data_downloadAdd.length() != 0)
		{
			//http://192.168.50.35:8080/DataMapManager/veTerminal/veidp.exe   ��ȡexe���֣�20180929135321_aaa.exe
			vector<string> v = split(data_downloadAdd, "/");
			string filename = v[v.size() - 1];

			char * strc = new char[strlen(filename.c_str()) + 1];
			strcpy(strc, filename.c_str());

			//�ӻ�����ȡtemp·�� C:\Users\week\AppData\Local\Temp\20180929135321_aaa.exe
			char buffer[128] = { 0 };
			const char* temp = Get_ENV("temp").c_str();
			sprintf(buffer, "%s\\%s", temp, strc);


			if (type == "1")	//1 ǿ�Ƹ���
			{
				ForkUtils::KillProcess(buffer);
				if (HttpDownFile(data_downloadAdd, buffer, data_hashCode) == 1)
				{
					WinExec(buffer, SW_SHOW);
				}
			}
			else if (type == "2")//2 ������£����� ��ʾ��
			{
				int ifupdate = MessageBox(0, TEXT("������µ����°汾"), TEXT("��ʾ"), 1);
				if (ifupdate == 1)//���ȷ�� ���� ������������� ��ֹ
				{
					ForkUtils::KillProcess(buffer);
					if (HttpDownFile(data_downloadAdd, buffer, data_hashCode) == 1)
					{
						WinExec(buffer, SW_SHOW);
					}
				}
				else if (ifupdate == 2)//���ȡ�� ����
				{
					delete strc;
					return;
				}
			}
			delete strc;
		}
	}
}


//�����µĺ����Ƚ�
bool CollectRoutinue::CompareUSBMap()
{
	auto current = USBUtils::GetUsbInstanceMap();
	bool bRet = true;
	//��С��ͬ��ֱ�ӷ�
	if (current.size() != mUsbmap.size())
	{
		bRet = false;
	}
	else
	{
		for (auto it = current.begin(); it != current.end(); it++)
		{
			auto fit = mUsbmap.find(it->first);
			//���ϼ����Ҳ�����ֱ�ӷ�
			if (fit == mUsbmap.end())
			{
				bRet = false;
				break;
			}
			//���ϼ��ϵĲ�ͬ��ֱ�ӷ�
			if (fit->second != it->second)
			{
				bRet = false;
				break;
			}
		}
	}

	mUsbmap = current;
	return bRet;
}

