// RepairPe.cpp: implementation of the CRepairPe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RepairPe.h"

#include <Tlhelp32.h>
#include <imagehlp.h>
#pragma comment (lib, "Dbghelp")

BYTE bCC = '\xCC';
BYTE bJmp[2] = {'\xff', '\xe0'};

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRepairPe::CRepairPe(CString strVir)
{
    m_StrVir = strVir;

    DumpVir(strVir.GetBuffer(0));
    
    BuildIat(strVir.GetBuffer(0), m_StrVir.GetBuffer(0));
    
    Repair(strVir.GetBuffer(0), m_StrVir.GetBuffer(0));
}

CRepairPe::~CRepairPe()
{

}

VOID CRepairPe::Dump(DEBUG_EVENT *pDe, DWORD dwEntryPoint)
{
    DWORD dwPid = pDe->dwProcessId;
  
    MODULEENTRY32 me32;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
    if ( INVALID_HANDLE_VALUE == hSnap )
    {
        return ;
    }

    me32.dwSize = sizeof(MODULEENTRY32);
    BOOL bRet = Module32First(hSnap, &me32);

    if ( FALSE == bRet )
    {
        CloseHandle(hSnap);
        return ;
    }

    HANDLE hFile = CreateFile(me32.szExePath, GENERIC_READ, 
                              FILE_SHARE_READ, NULL, 
                              OPEN_EXISTING, 
                              FILE_ATTRIBUTE_NORMAL, NULL);

    if ( INVALID_HANDLE_VALUE == hFile )
    {
        CloseHandle(hSnap);
        return ;
    }

    // 判断PE文件的有效性
    IMAGE_DOS_HEADER imgDos = { 0 };
    DWORD dwReadNum = 0;
    ReadFile(hFile, &imgDos, sizeof(IMAGE_DOS_HEADER), &dwReadNum, NULL);

    if ( imgDos.e_magic != IMAGE_DOS_SIGNATURE )
    {
        CloseHandle(hFile);
        CloseHandle(hSnap);
        return ;
    }

    SetFilePointer(hFile, imgDos.e_lfanew, 0, FILE_BEGIN);
    IMAGE_NT_HEADERS imgNt = { 0 };
    ReadFile(hFile, &imgNt, sizeof(IMAGE_NT_HEADERS), &dwReadNum, NULL);
    if ( imgNt.Signature != IMAGE_NT_SIGNATURE )
    {
        CloseHandle(hFile);
        CloseHandle(hSnap);
        return ;
    }

    // 得到EXE文件的大小
    DWORD BaseSize = me32.modBaseSize;

    if ( imgNt.OptionalHeader.SizeOfImage > BaseSize )
    {
        BaseSize = imgNt.OptionalHeader.SizeOfImage;
    }

    LPVOID pBase = VirtualAlloc(NULL, BaseSize, MEM_COMMIT, PAGE_READWRITE);

    if ( NULL == pBase )
    {
        CloseHandle(hSnap);
        return ;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    if ( NULL == hProcess )
    {
        VirtualFree(pBase, me32.modBaseSize, MEM_RELEASE);
        CloseHandle(hSnap);
        return ;
    }

    // 读取文件的数据
    bRet = ReadProcessMemory(hProcess, me32.modBaseAddr, pBase, me32.modBaseSize, NULL);
    if ( FALSE == bRet )
    {
        VirtualFree(pBase, me32.modBaseSize, MEM_RELEASE);
        CloseHandle(hProcess);
        CloseHandle(hSnap);
        return ;
    }

    PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pBase;

    if ( pDos->e_magic != IMAGE_DOS_SIGNATURE )
    {    
        VirtualFree(pBase, me32.modBaseSize, MEM_RELEASE);
        CloseHandle(hProcess);
        CloseHandle(hSnap);
        return ;
    }
    PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + (PBYTE)pBase);
    if ( pNt->Signature != IMAGE_NT_SIGNATURE )
    {
        VirtualFree(pBase, me32.modBaseSize, MEM_RELEASE);
        CloseHandle(hProcess);
        CloseHandle(hSnap);
        return ;
    }

    // 设置文件的入口地址
    pNt->OptionalHeader.AddressOfEntryPoint = dwEntryPoint - pNt->OptionalHeader.ImageBase;
    // 设置文件的对齐方式
    pNt->OptionalHeader.FileAlignment = 0x1000;
    PIMAGE_SECTION_HEADER pSec = (PIMAGE_SECTION_HEADER)((PBYTE)&pNt->OptionalHeader + 
        pNt->FileHeader.SizeOfOptionalHeader);

    for ( int i = 0; i < pNt->FileHeader.NumberOfSections; i ++ )
    {
        pSec->PointerToRawData = pSec->VirtualAddress;
        pSec->SizeOfRawData = pSec->Misc.VirtualSize;
        pSec ++;
    }
    CloseHandle(hFile);
    
    m_StrVir = m_StrVir.Left(m_StrVir.ReverseFind('\\'));
    m_StrVir += "\\dump.exe";

    hFile = CreateFile(m_StrVir.GetBuffer(0), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if ( INVALID_HANDLE_VALUE == hFile )
    {
        VirtualFree(pBase, me32.modBaseSize, MEM_RELEASE);
        CloseHandle(hProcess);
        CloseHandle(hSnap);
        return ;
    }
    DWORD dwWriteNum = 0;

    // 将读取的数据写入到文件
    bRet = WriteFile(hFile, pBase, me32.modBaseSize, &dwWriteNum, NULL);
    if ( dwWriteNum != me32.modBaseSize || 
         FALSE == bRet )
    {
        printf("Error! \r\n");
    }

    CloseHandle(hFile);
    VirtualFree(pBase, me32.modBaseSize, MEM_RELEASE);
    CloseHandle(hProcess);
    CloseHandle(hSnap);
}

VOID CRepairPe::ThreeCc(DEBUG_EVENT *pDe, BYTE *bCode)
{
    // 在入口点断下后
    // 恢复入口点的代码
    // 然后开始dump
    CONTEXT context;
    DWORD dwNum;
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 
                                  FALSE, pDe->dwProcessId);
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 
                                FALSE, pDe->dwThreadId);
    SuspendThread(hThread);

    BYTE bTmp;
    ReadProcessMemory(hProcess, 
        pDe->u.Exception.ExceptionRecord.ExceptionAddress,
        &bTmp, sizeof(BYTE), &dwNum);
    
    
    context.ContextFlags = CONTEXT_FULL;
    GetThreadContext(hThread, &context);

    WriteProcessMemory(hProcess, 
                pDe->u.Exception.ExceptionRecord.ExceptionAddress,
                bCode, sizeof(BYTE), &dwNum);

    context.Eip --;

    SetThreadContext(hThread, &context);

    Dump(pDe, context.Eip);
    
    ResumeThread(hThread);
    CloseHandle(hThread);
    CloseHandle(hProcess);
}

VOID CRepairPe::TwoCc(DEBUG_EVENT *pDe, BYTE *bCode)
{
    // 在xor ebx, ebx处断下后
    // 首先需要恢复xor ebx, ebx原来的字节码
    // 然后在修改xor ebx, ebx为jmp eax
    // 并在入口点设置断点
    CONTEXT context;
    DWORD dwNum;
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 
                                  FALSE, pDe->dwProcessId);
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 
                                FALSE, pDe->dwThreadId);
    SuspendThread(hThread);

    BYTE bTmp;
    ReadProcessMemory(hProcess, 
        pDe->u.Exception.ExceptionRecord.ExceptionAddress,
        &bTmp, sizeof(BYTE), &dwNum);
    
    context.ContextFlags = CONTEXT_FULL;
    GetThreadContext(hThread, &context);
    
    context.Eip --;
    WriteProcessMemory(hProcess, (void *)context.Eip, 
                       bJmp, sizeof(BYTE) * 2, &dwNum);

    ReadProcessMemory(hProcess, (const void *)context.Eax, 
                      bCode, sizeof(BYTE), &dwNum);

    WriteProcessMemory(hProcess, (void *)context.Eax, 
                       &bCC, sizeof(BYTE), &dwNum);

    SetThreadContext(hThread, &context);
       
    ResumeThread(hThread);
    CloseHandle(hThread);
    CloseHandle(hProcess);
}

VOID CRepairPe::OneCc(DEBUG_EVENT *pDe, BYTE *bCode)
{
    // 在jmp edi处断下后
    // 首先需要恢复jmp edi原来的字节码
    // 然后在还原完原入口代码的下一句代码处
    // 即xor ebx, ebx处设置断点
    CONTEXT context;
    DWORD dwNum;
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,
                                  FALSE, pDe->dwProcessId);
    HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 
                                FALSE, pDe->dwThreadId);
    SuspendThread(hThread);

    BYTE bTmp;
    ReadProcessMemory(hProcess, 
        pDe->u.Exception.ExceptionRecord.ExceptionAddress,
        &bTmp, sizeof(BYTE), &dwNum);
        
    context.ContextFlags = CONTEXT_FULL;
    GetThreadContext(hThread, &context);

    context.Eip --;
    WriteProcessMemory(hProcess, (void *)context.Eip, 
                       bCode, sizeof(BYTE), &dwNum);

    SetThreadContext(hThread, &context);

    DWORD dwEdi = context.Edi + 0x22;
    ReadProcessMemory(hProcess, (const void *)dwEdi, 
                      bCode, sizeof(BYTE), &dwNum);
    WriteProcessMemory(hProcess, (void *)dwEdi, 
                       &bCC, sizeof(BYTE), &dwNum);

    SetThreadContext(hThread, &context);
    
    ResumeThread(hThread);
    CloseHandle(hThread);
    CloseHandle(hProcess);
}

BOOL CRepairPe::DumpVir(char *strVir)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    si.cb = sizeof(si);
    GetStartupInfo(&si);
    
    DEBUG_EVENT de = { 0 };
    CONTEXT context = { 0 };
    
    // 创建病毒进程
    BOOL bRet = CreateProcess(strVir,
        NULL, NULL, NULL, FALSE,
        DEBUG_PROCESS | DEBUG_ONLY_THIS_PROCESS,
        NULL, NULL, &si, &pi);
    
    if ( bRet == FALSE )
    {
        return bRet;
    }
    
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    BYTE bCode;
    DWORD dwNum;

    // 第几次断点
    int nCc = 0;

    while ( TRUE )
    {
        // 开始调试循环
        WaitForDebugEvent(&de, INFINITE);
        
        switch ( de.dwDebugEventCode )
        {
        case CREATE_PROCESS_DEBUG_EVENT:
            {
                // 计算入口地址+0x58的地址
                // 即解密还原完后续病毒处的
                // jmp edi的地址处
                DWORD dwAddr = 0x58 + (DWORD)de.u.CreateProcessInfo.lpStartAddress;

                // 暂停线程
                SuspendThread(de.u.CreateProcessInfo.hThread);
                // 读取入口地址+0x58地址处的字节码
                ReadProcessMemory(de.u.CreateProcessInfo.hProcess, 
                                  (const void *)dwAddr, 
                                  &bCode, sizeof(BYTE), &dwNum);
                // 在入口地址+0x58地址处写入0xCC
                // 即写入INT 3
                WriteProcessMemory(de.u.CreateProcessInfo.hProcess, 
                                   (void *)dwAddr, &bCC, 
                                   sizeof(BYTE), &dwNum);
                // 恢复线程
                ResumeThread(de.u.CreateProcessInfo.hThread);

                break;
            }
        case EXCEPTION_DEBUG_EVENT:
            {
                switch ( nCc )
                {
                case 0:
                    {
                        // 第0次的断点是系统断点
                        // 这里忽略
                        nCc ++;
                        break;
                    }
                case 1:
                    {
                        OneCc(&de, &bCode);
                        nCc ++;
                        break;
                    }
                case 2:
                    {
                        TwoCc(&de, &bCode);
                        nCc ++;
                        break;
                    }
                case 3:
                    {
                        ThreeCc(&de, &bCode);
                        nCc ++;
                        goto end0;
                        break;
                    }
                case 4:
                    {
                        nCc ++;
                        goto end0;
                    }
                    
                }
            }
        }
        
        ContinueDebugEvent(de.dwProcessId, de.dwThreadId, DBG_CONTINUE);
    }
end0:
    
    bRet = TRUE;
    return bRet;
}

DWORD CRepairPe::Rva2Fa(PIMAGE_NT_HEADERS pImgNtHdr, LPVOID lpBase, DWORD dwRva)
{
    PIMAGE_SECTION_HEADER pImgSecHdr;

    pImgSecHdr = ImageRvaToSection(pImgNtHdr, lpBase, dwRva);

    return dwRva - pImgSecHdr->VirtualAddress + pImgSecHdr->PointerToRawData;
}

VOID CRepairPe::BuildIat(char *pSrc, char *pDest)
{
    getchar();
    PIMAGE_DOS_HEADER pSrcImgDosHdr, pDestImgDosHdr;
    PIMAGE_NT_HEADERS pSrcImgNtHdr, pDestImgNtHdr;
    PIMAGE_SECTION_HEADER pSrcImgSecHdr, pDestImgSecHdr;
    PIMAGE_IMPORT_DESCRIPTOR pSrcImpDesc, pDestImpDesc;

    HANDLE hSrcFile, hDestFile;
    HANDLE hSrcMap, hDestMap;
    LPVOID lpSrcBase, lpDestBase;

    hSrcFile = CreateFile(pSrc, GENERIC_READ, FILE_SHARE_READ, 
                          NULL, OPEN_EXISTING, 
                          FILE_ATTRIBUTE_NORMAL, NULL);
    if ( hSrcFile == INVALID_HANDLE_VALUE )
    {
        printf("GetLastError() = %d, %08x \r\n", GetLastError(), GetLastError());
        return ;
    }
    hDestFile = CreateFile(pDest, GENERIC_READ | GENERIC_WRITE, 
                           FILE_SHARE_READ, NULL, OPEN_EXISTING, 
                           FILE_ATTRIBUTE_NORMAL, NULL);

    hSrcMap = CreateFileMapping(hSrcFile, NULL, PAGE_READONLY, 0, 0, 0);
    hDestMap = CreateFileMapping(hDestFile, NULL, PAGE_READWRITE, 0, 0, 0);


    lpSrcBase = MapViewOfFile(hSrcMap, FILE_MAP_READ, 0, 0, 0);
    lpDestBase = MapViewOfFile(hDestMap, FILE_MAP_WRITE, 0, 0, 0);

    pSrcImgDosHdr = (PIMAGE_DOS_HEADER)lpSrcBase;
    pDestImgDosHdr = (PIMAGE_DOS_HEADER)lpDestBase;

    pSrcImgNtHdr = (PIMAGE_NT_HEADERS)((DWORD)lpSrcBase 
                                       + pSrcImgDosHdr->e_lfanew);
    pDestImgNtHdr = (PIMAGE_NT_HEADERS)((DWORD)lpDestBase 
                                       + pDestImgDosHdr->e_lfanew);

    pSrcImgSecHdr = (PIMAGE_SECTION_HEADER)((DWORD)&pSrcImgNtHdr->OptionalHeader 
                                            + pSrcImgNtHdr->FileHeader.SizeOfOptionalHeader);
    pDestImgSecHdr = (PIMAGE_SECTION_HEADER)((DWORD)&pDestImgNtHdr->OptionalHeader 
                                            + pDestImgNtHdr->FileHeader.SizeOfOptionalHeader);

    DWORD dwImpSrcAddr, dwImpDestAddr;

    dwImpSrcAddr = pSrcImgNtHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    dwImpDestAddr = pDestImgNtHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

    dwImpSrcAddr = (DWORD)lpSrcBase + Rva2Fa(pSrcImgNtHdr, lpSrcBase, dwImpSrcAddr);
    dwImpDestAddr = (DWORD)lpDestBase + Rva2Fa(pDestImgNtHdr, lpDestBase, dwImpDestAddr);

    // 定位导入表
    pSrcImpDesc = (PIMAGE_IMPORT_DESCRIPTOR)dwImpSrcAddr;
    pDestImpDesc = (PIMAGE_IMPORT_DESCRIPTOR)dwImpDestAddr;

    PIMAGE_THUNK_DATA pSrcImgThkDt, pDestImgThkDt;

    int n = 0;
    while ( pSrcImpDesc->Name && pDestImpDesc->Name )
    {
        n ++;
        char *pSrcImpName = (char*)((DWORD)lpSrcBase 
                    + Rva2Fa(pSrcImgNtHdr, lpSrcBase, pSrcImpDesc->Name));
        char *pDestImpName = (char*)((DWORD)lpDestBase 
                    + Rva2Fa(pDestImgNtHdr, lpDestBase, pDestImpDesc->Name));

        pSrcImgThkDt = (PIMAGE_THUNK_DATA)((DWORD)lpSrcBase 
                    + Rva2Fa(pSrcImgNtHdr, lpSrcBase, pSrcImpDesc->FirstThunk));
        pDestImgThkDt = (PIMAGE_THUNK_DATA)((DWORD)lpDestBase 
                    + Rva2Fa(pDestImgNtHdr, lpDestBase, pDestImpDesc->FirstThunk));

        // 赋值信息
        while ( *((DWORD *)pSrcImgThkDt) && *((DWORD *)pDestImgThkDt) )
        {
            DWORD dwIatAddr = *((DWORD *)pSrcImgThkDt);
            *((DWORD *)pDestImgThkDt) = dwIatAddr;

            pSrcImgThkDt ++;
            pDestImgThkDt ++;
        }

        pSrcImpDesc ++;
        pDestImpDesc ++;
    }

    UnmapViewOfFile(lpDestBase);
    UnmapViewOfFile(lpSrcBase);

    CloseHandle(hDestMap);
    CloseHandle(hSrcMap);

    CloseHandle(hDestFile);
    CloseHandle(hSrcFile);
}

DWORD CRepairPe::Align(DWORD dwSize, DWORD dwAlign)
{
    DWORD dwAlignSize;
    
    dwAlignSize = (dwSize % dwAlign);
    
    if ( dwAlignSize != 0 )
    {
        dwAlignSize = ((dwSize / dwAlign) + 1) * dwAlign;
    }
    else
    {
        dwAlignSize = (dwSize / dwAlign) * dwAlign;
    }
    
    return dwAlignSize;
}

VOID CRepairPe::Repair(char *pSrc, char *pDest)
{
    PIMAGE_DOS_HEADER pSrcImgDosHdr ;
    PIMAGE_NT_HEADERS pSrcImgNtHdr;
    PIMAGE_SECTION_HEADER pSrcImgSecHdr;
    PIMAGE_SECTION_HEADER pSrcLastImgSecHdr;
    WORD wSrcSecNum, wDestSecNum;
    
    HANDLE hSrcFile = CreateFile(pSrc, GENERIC_READ, FILE_SHARE_READ, 
                                 NULL, OPEN_EXISTING, 
                                 FILE_ATTRIBUTE_NORMAL, NULL);    
    if ( hSrcFile == INVALID_HANDLE_VALUE )
    {
        printf("GetLastError() = %d, %08x \r\n", GetLastError(), GetLastError());
        return ;
    }
    HANDLE hSrcMap = CreateFileMapping(hSrcFile, NULL, PAGE_READONLY, 0, 0, NULL);
    LPVOID lpSrcBase = MapViewOfFile(hSrcMap, FILE_MAP_READ, 0, 0, 0);

    
    pSrcImgDosHdr = (PIMAGE_DOS_HEADER)lpSrcBase;
    pSrcImgNtHdr = (PIMAGE_NT_HEADERS)((DWORD)lpSrcBase + pSrcImgDosHdr->e_lfanew);
    pSrcImgSecHdr = (PIMAGE_SECTION_HEADER)((DWORD)&pSrcImgNtHdr->OptionalHeader 
                    + pSrcImgNtHdr->FileHeader.SizeOfOptionalHeader);
    
    wSrcSecNum = pSrcImgNtHdr->FileHeader.NumberOfSections;
    
    pSrcLastImgSecHdr = pSrcImgSecHdr + wSrcSecNum - 1;
    
    DWORD dwSrcFileSize = GetFileSize(hSrcFile, NULL);
    DWORD dwSrcSecSize = pSrcLastImgSecHdr->PointerToRawData 
                    + pSrcLastImgSecHdr->SizeOfRawData;

    LPVOID lpBase;
    DWORD dwNum;
    if ( dwSrcFileSize > dwSrcSecSize )
    {
        lpBase = VirtualAlloc(NULL, (dwSrcFileSize - dwSrcSecSize), 
                              MEM_COMMIT, PAGE_READWRITE);
        SetFilePointer(hSrcFile, dwSrcSecSize, NULL, FILE_BEGIN);
        ReadFile(hSrcFile, lpBase, (dwSrcFileSize - dwSrcSecSize), 
                 &dwNum, NULL);
    }

    UnmapViewOfFile(lpSrcBase);
    CloseHandle(hSrcMap);
    CloseHandle(hSrcFile);

    HANDLE hDestFile = CreateFile(pDest, GENERIC_ALL, FILE_SHARE_READ, 
                                  NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    IMAGE_DOS_HEADER ImgDosHdr;
    IMAGE_NT_HEADERS ImgNtHdr;
    IMAGE_SECTION_HEADER ImgSecHdr, ImgSecHdr1, ImgSecHdr2;

    ReadFile(hDestFile, &ImgDosHdr, sizeof(IMAGE_DOS_HEADER), &dwNum, NULL);
    SetFilePointer(hDestFile, ImgDosHdr.e_lfanew, NULL, FILE_BEGIN);
    ReadFile(hDestFile, &ImgNtHdr, sizeof(IMAGE_NT_HEADERS), &dwNum, NULL);
    wDestSecNum = ImgNtHdr.FileHeader.NumberOfSections;
    SetFilePointer(hDestFile, sizeof(IMAGE_SECTION_HEADER) * (wDestSecNum - 2), NULL, FILE_CURRENT);
    ReadFile(hDestFile, &ImgSecHdr2, sizeof(IMAGE_SECTION_HEADER), &dwNum, NULL);
    ReadFile(hDestFile, &ImgSecHdr, sizeof(IMAGE_SECTION_HEADER), &dwNum, NULL);

    SetFilePointer(hDestFile, ((-1) * sizeof(IMAGE_SECTION_HEADER)), NULL, FILE_CURRENT);
    ZeroMemory(&ImgSecHdr1, sizeof(IMAGE_SECTION_HEADER));
    WriteFile(hDestFile, &ImgSecHdr1, sizeof(IMAGE_SECTION_HEADER), &dwNum, NULL);

    DWORD dwFileEnd = ImgSecHdr.PointerToRawData;
    SetFilePointer(hDestFile, dwFileEnd, NULL, FILE_BEGIN);
    SetEndOfFile(hDestFile);

    SetFilePointer(hDestFile, 0, NULL, FILE_END);
    WriteFile(hDestFile, lpBase, (dwSrcFileSize - dwSrcSecSize), &dwNum, NULL);

    ImgNtHdr.FileHeader.NumberOfSections --;
    ImgNtHdr.OptionalHeader.SizeOfImage = ImgSecHdr2.VirtualAddress 
                    + Align(ImgSecHdr2.Misc.VirtualSize, ImgNtHdr.OptionalHeader.SectionAlignment);
    
    SetFilePointer(hDestFile, ImgDosHdr.e_lfanew, NULL, FILE_BEGIN);
    WriteFile(hDestFile, &ImgNtHdr, sizeof(IMAGE_NT_HEADERS), &dwNum, NULL);

    CloseHandle(hDestFile);
}
