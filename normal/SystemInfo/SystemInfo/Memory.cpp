#include "iostream"
#include "windows.h"
#include "shlwapi.h"
#include  "iomanip"
#pragma comment(lib,"shlwapi.lib")
using namespace std;

void mainM() 
{
	SYSTEM_INFO si;
	ZeroMemory(&si, sizeof(si));
	GetSystemInfo(&si);

	TCHAR szPageSize[MAX_PATH];
	StrFormatByteSize(si.dwPageSize, szPageSize, MAX_PATH);

	DWORD dwMemSize = (DWORD)si.lpMaximumApplicationAddress - (DWORD)si.lpMinimumApplicationAddress;
	TCHAR szMemSize[MAX_PATH];
	StrFormatByteSize(dwMemSize, szMemSize, MAX_PATH);

	cout << "Virtual memory page size:" << szPageSize << endl;
	cout.fill('0');
	cout << "Minimum application address:0x"
		<< hex
		<< setw(8)
		<< (DWORD)si.lpMinimumApplicationAddress
		<< endl;
	cout << "Maximum application address:0x"
		<< hex << setw(8)
		<< (DWORD)si.lpMaximumApplicationAddress
		<< endl;

	cout << "Total available virtual memory:"
		<< szMemSize << endl;
}