
#include<Windows.h>
#include<stdio.h>
#include <shlobj.h>
/*
CSIDL_BITBUCKET   ����վ
CSIDL_CONTROLS   �������
CSIDL_DESKTOP   Windows   ����Desktop
CSIDL_DESKTOPDIRECTORY   Desktop��Ŀ¼
CSIDL_DRIVES   �ҵĵ���
CSIDL_FONTS   ����Ŀ¼
CSIDL_NETHOOD   �����ھ�
CSIDL_NETWORK   �����ھ�����Ŀ¼
CSIDL_PERSONAL   �ҵ��ĵ�
CSIDL_PRINTERS   ��ӡ��
CSIDL_PROGRAMS   ������
CSIDL_RECENT   ����򿪵��ĵ�
CSIDL_SENDTO   �����͵����˵���
CSIDL_STARTMENU   �����������˵���
CSIDL_STARTUP   ����Ŀ¼
CSIDL_TEMPLATES   �ĵ�ģ��
*/
static VOID GetSpecialFolderQS()
{
	WCHAR szDeskTop[MAX_PATH] = { 0 };
	WCHAR szFavourites[MAX_PATH] = { 0 };
	WCHAR szFonts[MAX_PATH] = { 0 };
	WCHAR szMyDocument[MAX_PATH] = { 0 };
	WCHAR szProgramFiles[MAX_PATH] = { 0 };
	WCHAR szPrograms[MAX_PATH] = { 0 };
	WCHAR szStartUp[MAX_PATH] = { 0 };
	WCHAR szWindows[MAX_PATH] = { 0 };

	// ʹ��SHGetSpecialFolderPath��ȡ����Ŀ¼·��
	SHGetSpecialFolderPath(NULL, szDeskTop, CSIDL_DESKTOP, FALSE);
	SHGetSpecialFolderPath(NULL, szFavourites, CSIDL_FAVORITES, FALSE);
	SHGetSpecialFolderPath(NULL, szFonts, CSIDL_FONTS, FALSE);
	SHGetSpecialFolderPath(NULL, szMyDocument, CSIDL_PERSONAL, FALSE);
	SHGetSpecialFolderPath(NULL, szProgramFiles, CSIDL_PROGRAM_FILES, FALSE);
	SHGetSpecialFolderPath(NULL, szPrograms, CSIDL_PROGRAMS, FALSE);
	SHGetSpecialFolderPath(NULL, szStartUp, CSIDL_STARTUP, FALSE);
	SHGetSpecialFolderPath(NULL, szWindows, CSIDL_WINDOWS, FALSE);
	printf("DeskTop:\t %ws\n", szDeskTop);
	printf("Favourites:\t %ws\n", szFavourites);
	printf("Fonts:\t %ws\n", szFonts);
	printf("My Document:\t %ws\n", szMyDocument);
	printf("Program Files:\t %ws\n", szProgramFiles);
	printf("Programs:\t %ws\n", szPrograms);
	printf("StartUp:\t %ws\n", szStartUp);
	printf("Windows:\t %ws\n", szWindows);
}