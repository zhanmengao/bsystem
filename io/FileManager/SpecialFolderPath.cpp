
#include<Windows.h>
#include<stdio.h>
#include <shlobj.h>
/*
CSIDL_BITBUCKET   回收站
CSIDL_CONTROLS   控制面板
CSIDL_DESKTOP   Windows   桌面Desktop
CSIDL_DESKTOPDIRECTORY   Desktop的目录
CSIDL_DRIVES   我的电脑
CSIDL_FONTS   字体目录
CSIDL_NETHOOD   网上邻居
CSIDL_NETWORK   网上邻居虚拟目录
CSIDL_PERSONAL   我的文档
CSIDL_PRINTERS   打印机
CSIDL_PROGRAMS   程序组
CSIDL_RECENT   最近打开的文档
CSIDL_SENDTO   “发送到”菜单项
CSIDL_STARTMENU   任务条启动菜单项
CSIDL_STARTUP   启动目录
CSIDL_TEMPLATES   文档模板
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

	// 使用SHGetSpecialFolderPath获取特殊目录路径
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