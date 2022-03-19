#ifndef _MM_LOAD_DLL_H_
#define _MM_LOAD_DLL_H_


#include <Windows.h>


typedef BOOL(__stdcall *typedef_DllMain)(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);


void ShowError(char *lpszText);

// ģ��LoadLibrary�����ڴ�DLL�ļ���������
// lpData: �ڴ�DLL�ļ����ݵĻ�ַ
// dwSize: �ڴ�DLL�ļ����ڴ��С
// ����ֵ: �ڴ�DLL���ص����̵ļ��ػ�ַ
LPVOID MmLoadLibrary(LPVOID lpData, DWORD dwSize);

// ����PE�ṹ,��ȡPE�ļ����ص��ڴ��ľ����С
// lpData: �ڴ�DLL�ļ����ݵĻ�ַ
// ����ֵ: ����PE�ļ��ṹ��IMAGE_NT_HEADERS.OptionalHeader.SizeOfImageֵ�Ĵ�С
DWORD GetSizeOfImage(LPVOID lpData);

// ���ڴ�DLL���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ���
// lpData: �ڴ�DLL�ļ����ݵĻ�ַ
// lpBaseAddress: �ڴ�DLL���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ��е��ڴ��ַ
// ����ֵ: �ɹ�����TRUE�����򷵻�FALSE
BOOL MmMapFile(LPVOID lpData, LPVOID lpBaseAddress);

// ����SectionAlignment
// dwSize: ��ʾδ����ǰ�ڴ�Ĵ�С
// dwAlignment: �����Сֵ
// ����ֵ: �����ڴ����֮���ֵ
DWORD Align(DWORD dwSize, DWORD dwAlignment);

// �޸�PE�ļ��ض�λ����Ϣ
// lpBaseAddress: �ڴ�DLL���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ��е��ڴ��ַ
// ����ֵ: �ɹ�����TRUE�����򷵻�FALSE
BOOL DoRelocationTable(LPVOID lpBaseAddress);

// ��дPE�ļ��������Ϣ
// lpBaseAddress: �ڴ�DLL���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ��е��ڴ��ַ
// ����ֵ: �ɹ�����TRUE�����򷵻�FALSE
BOOL DoImportTable(LPVOID lpBaseAddress);

// �޸�PE�ļ����ػ�ַIMAGE_NT_HEADERS.OptionalHeader.ImageBase
// lpBaseAddress: �ڴ�DLL���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ��е��ڴ��ַ
// ����ֵ: �ɹ�����TRUE�����򷵻�FALSE
BOOL SetImageBase(LPVOID lpBaseAddress);

// ����DLL����ں���DllMain,������ַ��ΪPE�ļ�����ڵ�IMAGE_NT_HEADERS.OptionalHeader.AddressOfEntryPoint
// lpBaseAddress: �ڴ�DLL���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ��е��ڴ��ַ
// ����ֵ: �ɹ�����TRUE�����򷵻�FALSE
BOOL CallDllMain(LPVOID lpBaseAddress);

// ģ��GetProcAddress��ȡ�ڴ�DLL�ĵ�������
// lpBaseAddress: �ڴ�DLL�ļ����ص������еļ��ػ�ַ
// lpszFuncName: ��������������
// ����ֵ: ���ص��������ĵĵ�ַ
LPVOID MmGetProcAddress(LPVOID lpBaseAddress, PCHAR lpszFuncName);

// �ͷŴ��ڴ���ص�DLL�������ڴ�Ŀռ�
// lpBaseAddress: �ڴ�DLL���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ��е��ڴ��ַ
// ����ֵ: �ɹ�����TRUE�����򷵻�FALSE
BOOL MmFreeLibrary(LPVOID lpBaseAddress);


#endif