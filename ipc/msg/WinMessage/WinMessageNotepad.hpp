#pragma once
#include<Windows.h>
#include<string>
class MessageNotePad
{
public:
	void Open()
	{
		WinExec("notepad.exe", SW_SHOW);
	}
	void Close()
	{
		HWND hWnd = FindWindowA("Notepad", NULL);
		if (hWnd == NULL)
		{
			MessageBoxA(NULL, "没有找到记事本", "", MB_OK);
			return;
		}
		SendMessage(hWnd, WM_CLOSE, NULL, NULL);
	}
	std::string GetNotepadTitle()
	{
		HWND hWnd = FindWindow(L"Notepad", NULL);
		if (hWnd == NULL)
		{
			MessageBoxA(NULL, "没有找到记事本", "", MB_OK);
			return "";
		}
		char pCaptionText[MAXBYTE] = { 0 };
		SendMessage(hWnd, WM_GETTEXT, (WPARAM)MAXBYTE, (LPARAM)pCaptionText);
		MessageBoxA(NULL, pCaptionText, "", MB_OK);
		return pCaptionText;
	}
	void SetTitle(const char* name)
	{
		HWND hWnd = FindWindow(NULL, L"无标题 - 记事本");
		if (hWnd == NULL)
		{
			MessageBoxA(NULL, "没有找到记事本", "", MB_OK);
			return;
		}
		char buf[MAX_PATH] = { 0 };
		strcpy(buf, name);
		SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)buf);
	}
};