#include"../../../Setting/Service/ServiceCtrl.h"
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#include"../../../Process/WindowsProcessHelper/CreateProcess/SessionManager.h"
#include"../../../Process/WSessionManager/SessionManager/SessionFork.h"
#include"../../../Process/WindowsProcessHelper/Fork/ForkUtils.h"
#include"../../../../C++space/LogManager/LogManager/LogManager.hpp"
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"../../../Setting/Readini/INIManager.hpp"
#include"../../../../Network/HTTP/CURL/curl/DownloadTask.h"
#include"../../../ZIP/ZIPUtils/ZIPUtils.h"
#define __DEBUG 0
#pragma comment(lib,"libcurl_imp.lib")
#pragma comment ( lib, "ws2_32.lib" )
#pragma comment ( lib, "wldap32.lib" )
void main()
{
	//DoTask();
	//�����Լ����ػ�����
	ServiceCtrl::StartUpService(PathHelper::GetModuleFilePath());
}
enum UPDATE_TYPE
{
	UPDATE_FORCE = 1,				//ǿ������
	UPDATE_MATCH = 2,				//�ԱȰ汾�Ÿ���
};
//�߼��������ӽ���
void DoTask()
{
	auto idpPath = PathHelper::GetModuleFolder() + _T("veidp.exe");
	auto logPath = PathHelper::GetModuleFolder() + _T("\\log\\");
#ifdef _UNICODE
	LogManager *pLog = new LogManager(Convert::WStringToString(logPath));
#else
	LogManager *pLog = new LogManager(logPath);
#endif
	pLog->SetTag("Service");
	//�Զ�����
	//��ȡ�汾��Ϣ
	iniManager ini(PathHelper::GetModuleFolder() + _T("veidp.ini"));
	stdstr current = ini.ReadString(_T("version"), _T("current"));
	stdstr newversion = ini.ReadString(_T("version"), _T("newversion"));
	int updateType = ini.ReadNum(_T("version"), _T("type"));
	pLog->LogDaily("Run current : " + Convert::WStringToString(current) + "	newversion :" + Convert::WStringToString(newversion));
	bool bUpdate = false;

	if (newversion == _T("*") || current == newversion)
	{
		bUpdate = false;
	}
	//�汾��Ϣ��ƥ�䣬���Ը���
	else if (updateType == UPDATE_FORCE)
	{
		bUpdate = true;
	}
	else if (updateType == UPDATE_MATCH)
	{
		//�и��ַ���
		vector<stdstr> curvec;
		StrUtils::Split(current, curvec, '.');
		vector<stdstr> newvec;
		StrUtils::Split(newversion, newvec, '.');
		if (curvec.size() >= 3 && newversion.size() >= 3)
		{
			int curmainVersion = _wtoi(curvec[0].c_str());
			int cursubVersion = _wtoi(curvec[1].c_str());
			int curThrVersion = _wtoi(curvec[2].c_str());

			int newmainVersion = _wtoi(newvec[0].c_str());
			int newsubVersion = _wtoi(newvec[1].c_str());
			int newThrVersion = _wtoi(newvec[2].c_str());

			if (curmainVersion < newmainVersion)
			{
				bUpdate = true;
			}
			else if (curmainVersion > newmainVersion)
			{
				bUpdate = false;
			}
			else
			{
				if (cursubVersion < newsubVersion)
				{
					bUpdate = true;
				}
				else if (cursubVersion > newsubVersion)
				{
					bUpdate = false;
				}
				else
				{
					if (curThrVersion < newThrVersion)
					{
						bUpdate = true;
					}
					else if (curThrVersion > newThrVersion)
					{
						bUpdate = false;
					}
					else
					{
						bUpdate = false;
					}
				}
			}
		}
	}
	if (bUpdate)
	{
		std::string url = Convert::WStringToString(ini.ReadString(_T("version"), _T("url")));
		std::string md5 = Convert::WStringToString(ini.ReadString(_T("version"), _T("md5")));
		std::string localPath = Convert::WStringToString(PathHelper::GetModuleFolder() + newversion + _T(".zip"));
		pLog->LogDaily("Version update:	local:" + localPath + "	url :" + url);
		//�����ļ�
		DownloadTask task;
		task.AddSeed(CppConvert::ANSIToUTF8(url).c_str(), localPath);
		task.BeginTaskSync();
		pLog->LogDaily("Version update:	downloadSize:" + std::to_string(task.GetTaskDownloadSize()));
		//������� �Ա�MD5 TODO

		if (task.GetTaskDownloadSize() > 0)
		{
			//��ѹ�ļ�
			std::string moduleFolder = Convert::WStringToString(PathHelper::GetModuleFolder());
			std::string newFolder = moduleFolder + "newversion";
			UnzipFile(localPath, newFolder);

			//����Ŀ¼���滻�ļ�
			FileManager::CopyDirectory(newFolder, moduleFolder);

			ini.WriteString(_T("version"), _T("current"), newversion);

			//����ļ�
			FileManager::RemoveFloder(newFolder);
			FileManager::RemoveFile(localPath);
		}
	}

	DWORD sid = SessionManager::GetActiveSessionID();
	pLog->LogDaily("Service Create Process To: " + to_string(sid));
	if (sid > 0)
	{
		DWORD dwRet = SessionForkUtils::CreateEXE_Session(idpPath, sid, true, pLog);
		if (pLog)
		{
			pLog->Log("CreateEXE_Me Return " + to_string(dwRet));
		}
		if (dwRet != 0)
		{
			return;
		}
	}
	if (pLog)
	{
		delete pLog;
		pLog = nullptr;
	}
}