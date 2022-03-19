#include "Logic.h"
#include "Definer.h"
#include "Tools.h"
using namespace std;

HANDLE handles[2];
void StartThread()
{
	handles[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadUser, NULL, 1, 0); //�������߳�
	ResumeThread(handles[0]);  //�������߳�

	handles[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFifo, NULL, 1, 0); //�������߳�
	ResumeThread(handles[1]);  //�������߳�
}
//�߳�1 ���ܿ���̨����
void ThreadUser()
{
	//�߳����
	char buffer[64];
	while (1)
	{
		cout << "������ָ��" << endl;
		cin >> buffer;
		if (strcmp(buffer, "reload") == 0)
		{
			LoadConfig();
		}
	}
}
//�߳�2 ��ʱͨ��IPC��log�ļ���������
void ThreadFifo()
{
	char buffer[1024];
	DWORD WriteNum;
	SYSTEMTIME sys;
	while (1)
	{
		//һСʱ����һ��
		Sleep(3600000);
		//��ȡ��ǰʱ��
		GetLocalTime(&sys);
		if (sys.wHour == GlobalDefine::wSendHour && sys.wSecond == GlobalDefine::wSendMin)
		{
			//��ͨ�ܵ�
			if (WaitNamedPipe(TEXT("\\\\.\\Pipe\\mypipe"), NMPWAIT_WAIT_FOREVER) == FALSE)
			{
				cout << "�ȴ������ܵ�ʵ��ʧ�ܣ�" << endl;
				continue;
			}
			HANDLE hPipe = CreateFile(TEXT("\\\\.\\Pipe\\mypipe"),
				GENERIC_READ | GENERIC_WRITE, 0, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hPipe == INVALID_HANDLE_VALUE)
			{
				cout << "���������ܵ�ʧ�ܣ�" << endl;
				CloseHandle(hPipe);
				continue;
			}
			//���ļ�
			ifstream ifs;
			ifs.open(GlobalDefine::saveDataPath, ios::in);
			if (!ifs.is_open())
			{
				cout << "Open saveData.txt error" << endl;
				cout << "�رչܵ���" << endl;
				CloseHandle(hPipe);
				break;
			}	
			while (!ifs.eof())
			{
				memset(buffer, 0, sizeof(buffer));
				ifs.read(buffer, sizeof(buffer));
				if (WriteFile(hPipe, buffer, strlen(buffer), &WriteNum, NULL) == false)
				{
					cout << "����д��ܵ�ʧ�ܣ�" << endl;
					cout << "�رչܵ���" << endl;
					CloseHandle(hPipe);
					break;
				}
			}
		}
	}

	ExitThread(0);
}

void fileWatcher()
{
	DWORD cbBytes;
	char file_name[MAX_PATH]; //�����ļ���;
	char file_rename[MAX_PATH]; //�����ļ��������������;
	char notify[1024];
	TCHAR *dir = _T("C:/");

	HANDLE dirHandle = CreateFile(dir,
		GENERIC_READ | GENERIC_WRITE | FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);

	//�������ض����Ŀ���ļ�ϵͳ��֧�ָò���������ʧ�ܣ�ͬʱ����GetLastError()����ERROR_INVALID_FUNCTION
	if (dirHandle == INVALID_HANDLE_VALUE)
	{
		cout << "error" + GetLastError() << endl;
	}

	memset(notify, 0, sizeof(notify));
	FILE_NOTIFY_INFORMATION *pnotify = (FILE_NOTIFY_INFORMATION*)notify;

	cout << "Start Monitor..." << endl;

	while (true)
	{
		if (ReadDirectoryChangesW(dirHandle, &notify, 1024, true,
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
			FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_ACCESS,
			&cbBytes, NULL, NULL))
		{
			if (false && CheckFile(std::wstring(pnotify->FileName)) == false)
			{
				continue;
			}

			//��ȡ�ļ���
			if (pnotify->FileName)
			{
				memset(file_name, 0, sizeof(file_name));
				WideCharToMultiByte(CP_ACP, 0, pnotify->FileName, pnotify->FileNameLength / 2, file_name, 99, NULL, NULL);
			}
			string log;
			//�������͹�����,�����ļ����������ġ�ɾ������������;
			switch (pnotify->Action)
			{
			case FILE_ACTION_ADDED:
				cout << "file add:" << file_name << endl;
				log += "file add:";
				log += file_rename;
				break;

			case FILE_ACTION_MODIFIED:
			{
				//�޸��ļ�����������飬�Ƿ��ظ��޸�
				if (!IsModNeedSave(file_name))
				{
					continue;
				}
				cout << "file modified:" << file_name << endl;
				log += "file modified:";
				log += file_rename;
				break;
			}
			case FILE_ACTION_REMOVED:
				cout << "file removed:" << file_name << endl;
				log += "file removed:";
				log += file_rename;
				break;

			case FILE_ACTION_RENAMED_OLD_NAME:
			{
				//��ȡ���������ļ���;
				if (pnotify->NextEntryOffset != 0 && (pnotify->FileNameLength > 0 && pnotify->FileNameLength < MAX_PATH))
				{
					PFILE_NOTIFY_INFORMATION p = (PFILE_NOTIFY_INFORMATION)((char*)pnotify + pnotify->NextEntryOffset);
					memset(file_rename, 0, sizeof(file_rename));
					WideCharToMultiByte(CP_ACP, 0, p->FileName, p->FileNameLength / 2, file_rename, 99, NULL, NULL);
				}
				cout << "file renamed:" << file_name << "->" << file_rename << endl;
				log += "file renamed:";
				log += file_name;
				log += "->";
				log += file_rename;
			}
				break;

			default:
				cout << "UNknow command!" << endl;
			}
			//��log
			if (!log.empty())
			{
				//SaveDataToFile(GlobalDefine::saveDataPath, log);
			}
		}
	}

	CloseHandle(dirHandle);
}

//����ļ��Ƿ�Ϸ�
bool CheckFile(const wstring& filePath)
{
	if (GlobalDefine::allWatch)
	{
		return true;
	}
	auto findPos = filePath.find('.');
	if (findPos == filePath.length())
	{
		return false;
	}
	std::wstring p2 = filePath.substr(findPos+1, filePath.length());
	if (p2 == L"dos")
	{
		return GlobalDefine::watchDOS;
	}
	else if (p2 == L"xls")
	{
		return GlobalDefine::watchXLC;
	}
	else if (p2 == L"pdf")
	{
		return GlobalDefine::watchPDF;
	}
	else if (p2 == L"txt")
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;
}

bool IsModNeedSave(const string& filePath)
{
	auto it = GlobalData::modifiedData.find(filePath);
	if (it == GlobalData::modifiedData.end())
	{
		GlobalData::modifiedData.insert(filePath);
		return true;
	}
	else
	{
		return false;
	}
}