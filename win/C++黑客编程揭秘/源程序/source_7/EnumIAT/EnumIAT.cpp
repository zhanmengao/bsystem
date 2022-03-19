// EnumIAT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <Dbghelp.h>
#include <stdio.h>

#pragma comment (lib, "Dbghelp")

int main(int argc, char* argv[])
{
    HANDLE hFile = CreateFile("test.exe", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if ( hFile == INVALID_HANDLE_VALUE )
    {
        printf("CreateFile \r\n");
        return -1;
    }

    HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY | SEC_IMAGE, 0, 0, 0);
    if ( hMap == NULL )
    {
        CloseHandle(hFile);
        printf("CreateFileMapping \r\n");
        return -1;
    }

    LPVOID lpBase = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
    if ( lpBase == NULL )
    {
        CloseHandle(hMap);
        CloseHandle(hFile);
        printf("MapViewOfFile \r\n");
        return -1;
    }

    PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)lpBase;

    if ( pDosHdr->e_magic != IMAGE_DOS_SIGNATURE )
    {
        printf("IMAGE_DOS_SIGNATURE \r\n");

        UnmapViewOfFile(lpBase);
        
        CloseHandle(hMap);
        
        CloseHandle(hFile);
        return -1;
    }

    PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)((DWORD)lpBase + pDosHdr->e_lfanew);
    if ( pNtHdr->Signature != IMAGE_NT_SIGNATURE )
    {
        printf("IMAGE_NT_SIGNATURE \r\n");

        UnmapViewOfFile(lpBase);
        
        CloseHandle(hMap);
        
        CloseHandle(hFile);
        return -1;
    }

    DWORD dwNum = 0;
    PIMAGE_IMPORT_DESCRIPTOR pImpDes = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(lpBase, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &dwNum);

    PIMAGE_IMPORT_DESCRIPTOR pTmpImpDes = pImpDes;
    while ( pTmpImpDes->Name )
    {
        printf("DllName = %s \r\n", (DWORD)lpBase + (DWORD)pTmpImpDes->Name);
        PIMAGE_THUNK_DATA thunk = (PIMAGE_THUNK_DATA)(pTmpImpDes->FirstThunk + (DWORD)lpBase);

        int n = 0;
        while ( thunk->u1.Function )
        {
            if ( thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG )
            {
                printf("Ordinal = %08X \r\n", thunk->u1.Ordinal & 0xFFFF);
            } 
            else
            {
                PIMAGE_IMPORT_BY_NAME pImName = (PIMAGE_IMPORT_BY_NAME)thunk->u1.Function;
                printf("FuncName = %s \t \t", (DWORD)lpBase + pImName->Name);
                DWORD dwAddr = (DWORD)((DWORD *)((DWORD)pNtHdr->OptionalHeader.ImageBase + pTmpImpDes->FirstThunk) + n);
                printf("addr = %08x \r\n", dwAddr);
            }

            thunk ++;
            n ++;
        }

        pTmpImpDes ++;
    }

    UnmapViewOfFile(lpBase);

    CloseHandle(hMap);

    CloseHandle(hFile);

    getchar();

	return 0;
}
