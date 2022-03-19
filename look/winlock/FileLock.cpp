#include <Windows.h>
#include <iostream>
#include<tchar.h>

using namespace std;

#define FILENAME "D://a.txt"
void FileLockMain()
{
	OVERLAPPED Oapped;
	FILE*fp = NULL;
	HANDLE hDir = CreateFile(_T(FILENAME),                   // lpFileName
		GENERIC_WRITE | GENERIC_READ,               // dwDesiredAccess
		0,                          // dwShareMode
		NULL,                       // lpSecurityAttributes
		CREATE_ALWAYS,              // dwCreationDisposition
		FILE_ATTRIBUTE_NORMAL, // dwFlagsAndAttributes
		NULL);

	int i = LockFileEx(hDir, LOCKFILE_FAIL_IMMEDIATELY, (DWORD)0, (DWORD)200, (DWORD)0, &Oapped);

	if (i != 0)
	{
		cout << "file lock success!!" << endl;
		Sleep(10000);
	}


	int n = UnlockFileEx(hDir, (DWORD)0, (DWORD)200, (DWORD)0, &Oapped);

	CloseHandle(hDir);

	if (n != 0)
	{
		cout << "file unlock success!!" << endl;

		Sleep(1000);

		if ((fp = fopen("e:\\test.txt", "w")) == NULL)
		{
			cout << "Can't open !" << endl;
		}
		else
		{
			cout << "open success!";
		}
	}
	return;
}