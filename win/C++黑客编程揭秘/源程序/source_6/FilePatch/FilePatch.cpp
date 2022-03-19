// FilePatch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    // VA = 00401EA8
    // FileOffset = 00001EA8
    DWORD dwFileOffset = 0x00001EA8;
    BYTE  bCode = 0;
    DWORD dwReadNum = 0;

    // �жϲ���
    if ( argc != 2 )
    {
        printf("Please input two argument \r\n");
        return -1;
    }

    // ���ļ�
    HANDLE hFile = CreateFile(argv[1],
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

    if ( hFile == INVALID_HANDLE_VALUE )
    {
        return -1;
    }

    SetFilePointer(hFile, dwFileOffset, 0, FILE_BEGIN);

    ReadFile(hFile, (LPVOID)&bCode, sizeof(BYTE), &dwReadNum, NULL);

    // �Ƚϵ�ǰλ���Ƿ�ΪJNZ
    if ( bCode != '\x75' )
    {
        printf("%02X \r\n", bCode);
        CloseHandle(hFile);
        return -1;
    }
    
    // �޸�ΪJZ
    bCode = '\x74';
    SetFilePointer(hFile, dwFileOffset, 0, FILE_BEGIN);
    WriteFile(hFile, (LPVOID)&bCode, sizeof(BYTE), &dwReadNum, NULL);

    printf("Write JZ is Successfully ! \r\n");

    CloseHandle(hFile);

    // ����
    WinExec(argv[1], SW_SHOW);

    getchar();

	return 0;
}
