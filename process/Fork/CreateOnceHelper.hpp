#pragma once
#include<Windows.h>
#include<string>
#include<fileapi.h>
#include<iostream>
#include<tchar.h>
#include<Sddl.h>
#include<strsafe.h>
#pragma comment(lib,"Advapi32.lib")
using namespace std;

class CreateOnceHelp
{
public:
	CreateOnceHelp(const wstring& fName):m_Fname(fName)
	{
	}
	~CreateOnceHelp()
	{
		UnLock();
	}
	bool Lock()
	{
		OVERLAPPED Oapped;
		hDir = CreateFile(m_Fname.c_str(),                   // lpFileName
			GENERIC_WRITE | GENERIC_READ,               // dwDesiredAccess
			0,                          // dwShareMode
			NULL,                       // lpSecurityAttributes
			OPEN_ALWAYS,              // dwCreationDisposition
			FILE_ATTRIBUTE_NORMAL, // dwFlagsAndAttributes
			NULL);

		int i = LockFileEx(hDir, LOCKFILE_FAIL_IMMEDIATELY, (DWORD)0, (DWORD)200, (DWORD)0, &Oapped);
		if (i != 0)
		{
			cout << "file lock success!!" << endl;
			return true;
		}
		return false;
	}
	void ChangeState(int state)
	{
		DWORD dwRet;
		wstring str = to_wstring(state);
		SetFilePointer(hDir, 0, NULL, FILE_END);
		if (!WriteFile(hDir, str.c_str(), str.size(), &dwRet, NULL))
		{
			cout << "WriteFile error!" << to_string(GetLastError()) << endl;
		}
		else
		{
			DWORD fSize = GetFileSize(hDir, NULL);
			cout << "WriteFile sec!" << to_string(fSize) << endl;
		}
	}
	int ReadState()
	{
		WCHAR buf[1024] = { 0 };
		DWORD dwRet = 0;
		DWORD fSize = GetFileSize(hDir, NULL);
		cout << "Read sec!" << to_string(fSize) << endl;
		return fSize % 2;
	}
	void UnLock()
	{
		if (hDir != NULL)
		{
			OVERLAPPED Oapped;
			int n = UnlockFileEx(hDir, (DWORD)0, (DWORD)200, (DWORD)0, &Oapped);
			CloseHandle(hDir);
			hDir = NULL;
			if (n != 0)
			{
				cout << "file unlock success!!" << endl;
			}
		}
	}
private:
	wstring m_Fname;
	HANDLE hDir;
};

class OnceLock final
{
public:
	OnceLock(const wstring& lockName):mLockName(lockName)
	{

	}
	~OnceLock()
	{
		UnLock();
	}
	bool Lock()
	{
		hMutex = CreateMutex(NULL, TRUE, mLockName.c_str());
		if (hMutex)
		{
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				return false;
			}
		}
		return true;
	}
	void UnLock()
	{
		if (hMutex)
		{
			ReleaseMutex(hMutex);
		}
	}
private:
	HANDLE hMutex;
	wstring mLockName;
};

class OnceLockEx final
{
public:
	OnceLockEx(const wstring& lockName) :mSpaceName(lockName)
	{
		
	}
	~OnceLockEx()
	{
		UnLock();
	}
	bool Lock()
	{
		bool bRet = true;
		SECURITY_ATTRIBUTES sa;
		do
		{
			//边界对象
			hBound = CreateBoundaryDescriptor(mBoundName.c_str(), 0);
			if (hBound == NULL || hBound == INVALID_HANDLE_VALUE)
			{
				bRet = false;
				break;
			}

			//sid
			BYTE localAdminSID[SECURITY_MAX_SID_SIZE];
			PSID pLocalAdminSID = &localAdminSID;
			DWORD cbSID = sizeof(localAdminSID);
			if (!CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, pLocalAdminSID, &cbSID))
			{
				bRet = false;
				break;
			}


			//关联
			if (!AddSIDToBoundaryDescriptor(&hBound, pLocalAdminSID))
			{
				bRet = false;
				break;
			}

			//在本地管理员组创建命名空间
			sa.nLength = sizeof(sa);
			sa.bInheritHandle = FALSE;
			if (!ConvertStringSecurityDescriptorToSecurityDescriptor(TEXT("D:(A;GA;;;BA)"),
				SDDL_REVISION_1, &sa.lpSecurityDescriptor, NULL))
			{
				bRet = false;
				break;
			}
			//创建专属命名空间
			hNameSpace = CreatePrivateNamespace(&sa, hBound, mSpaceName.c_str());
			if (hNameSpace == NULL || hNameSpace == INVALID_HANDLE_VALUE)
			{
				bRet = false;
				break;
			}
			
			//创建专属命名空间下的锁
			TCHAR szMutexName[64] = { 0 };
			StringCchPrintf(szMutexName, _countof(szMutexName), TEXT("%s\\%s"), 
				mSpaceName.c_str(), TEXT("Singleton"));
			hMutex = CreateMutex(NULL, TRUE, szMutexName);
			if (hMutex)
			{
				if (GetLastError() == ERROR_ALREADY_EXISTS)
				{
					bRet = false;
					break;
				}
			}
		} while (0);
		if (sa.lpSecurityDescriptor)
		{
			LocalFree(sa.lpSecurityDescriptor);
			sa.lpSecurityDescriptor = NULL;
		}
		if (!bRet)
		{
			UnLock();
		}
		return bRet;
	}
	void UnLock()
	{
		if (hMutex)
		{
			ReleaseMutex(hMutex);
			hMutex = NULL;
		}
		if (hNameSpace)
		{
			ClosePrivateNamespace(hNameSpace, PRIVATE_NAMESPACE_FLAG_DESTROY);
			hNameSpace = NULL;
		}
		if (hBound)
		{
			CloseHandle(hBound);
			hBound = NULL;
		}
	}
private:
	HANDLE hBound = NULL;
	HANDLE hMutex;
	HANDLE hNameSpace = NULL;

	wstring mBoundName = L"OnceLockEx";
	wstring mSpaceName = L"MyNameSpace";
};