#pragma once
#ifndef _LNKUTILS_H
#define _LNKUTILS_H
#include"../../../C++space/StringTools/StringTools/StringHead.h"
#include<Windows.h>

int CleanDesktopShortcut();
int DeleteDesktopShortcut(TTCHAR* lpszShortcut);
int CreateLink(TTCHAR* szPath, TTCHAR* szArguments, WCHAR* szLink, TTCHAR* szLinkICO);
BOOL CreateDesktopShortcut(TTCHAR* szPath, TTCHAR* szArguments, WCHAR* szLinkName, TTCHAR* szLinkICO);
bool  SetWallpaper(const wchar_t* szPath, DWORD dwStyle);
#ifdef __cplusplus
HRESULT GetWallpaper(std::wstring& wallpaper);
#endif
#endif