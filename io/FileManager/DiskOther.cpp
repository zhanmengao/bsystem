#include<Windows.h>
#include<stdio.h>
#include<tchar.h>
/******************************************************************************
* Function: get disk's drive letters from physical number
*           e.g. 0-->{C, D, E} (disk0 has 3 drives, C:, D: and E:)
* input: phyDriveNumber, disk's physical number
* output: letters, letters array
* return: Succeed, the amount of letters
*         Fail, -1
******************************************************************************/
DWORD GetPartitionLetterFromPhysicalDrive(DWORD phyDriveNumber, TCHAR **letters)
{
	DWORD mask;
	DWORD driveType;
	DWORD bmLetters;
	DWORD diskNumber;
	TCHAR path[1024] = { 0 };
	TCHAR letter;
	DWORD letterNum;
	WORD i;
	TCHAR *p;

	bmLetters = GetLogicalDrives();
	if (0 == bmLetters)
	{
		return (DWORD)-1;
	}

	letterNum = 0;
	for (i = 0; i < sizeof(DWORD) * 8; i++)
	{
		mask = 0x1u << i;
		if ((mask & bmLetters) == 0)        //get one letter
		{
			continue;
		}
		letter = (TCHAR)(0x41 + i);    //ASCII change
		wsprintf(path, _T("%c:\\"), letter);
		driveType = GetDriveType(path);
		if (driveType != DRIVE_FIXED)
		{
			bmLetters &= ~mask;     //clear this bit
			continue;
		}
		//diskNumber = GetPhysicalDriveFromPartitionLetter(letter);
		if (diskNumber != phyDriveNumber)
		{
			bmLetters &= ~mask;     //clear this bit
			continue;
		}
		letterNum++;
	}

	//build the result
	*letters = (TCHAR *)malloc(letterNum);
	if (NULL == *letters)
	{
		return (DWORD)-1;
	}
	p = *letters;
	for (i = 0; i < sizeof(DWORD) * 8; i++)
	{
		mask = 0x1u << i;
		if ((mask & bmLetters) == 0)
		{
			continue;
		}
		letter = (CHAR)(0x41 + i);    //ASCII change
		*p = letter;
		p++;
	}

	return letterNum;
}

/******************************************************************************
* Function: get the disk's drive layout infomation
* input: disk, disk name
* output: drive layout info
* return: Succeed, 0
*         Fail, -1
******************************************************************************/
DWORD GetDiskDriveLayout(const TCHAR *disk, DRIVE_LAYOUT_INFORMATION_EX *driveLayout)
{
	HANDLE hDevice;               // handle to the drive to be examined
	BOOL result;                  // results flag
	DWORD readed;                 // discard results

	hDevice = CreateFile(
		disk, // drive to open
		GENERIC_READ | GENERIC_WRITE,     // access to the drive
		FILE_SHARE_READ | FILE_SHARE_WRITE, //share mode
		NULL,             // default security attributes
		OPEN_EXISTING,    // disposition
		0,                // file attributes
		NULL            // do not copy file attribute
	);
	if (hDevice == INVALID_HANDLE_VALUE) // cannot open the drive
	{
		fprintf(stderr, "CreateFile() Error: %ld\n", GetLastError());
		return DWORD(-1);
	}

	result = DeviceIoControl(
		hDevice,               // handle to device
		IOCTL_DISK_GET_DRIVE_LAYOUT_EX, // dwIoControlCode
		NULL,                           // lpInBuffer
		0,                              // nInBufferSize
		driveLayout,           // output buffer
		sizeof(*driveLayout),         // size of output buffer
		&readed,      // number of bytes returned
		NULL     // OVERLAPPED structure
	);
	if (!result)
	{
		fprintf(stderr, "IOCTL_DISK_GET_DRIVE_LAYOUT_EX Error: %ld\n", GetLastError());
		(void)CloseHandle(hDevice);
		return DWORD(-1);
	}

	(void)CloseHandle(hDevice);
	return 0;
}

void mainDisk()
{
	DRIVE_LAYOUT_INFORMATION_EX *pLy = NULL;
	GetDiskDriveLayout(L"C:", pLy);
	system("pause");
}