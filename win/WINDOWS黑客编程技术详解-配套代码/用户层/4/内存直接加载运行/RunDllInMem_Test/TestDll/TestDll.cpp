// TestDll.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"


BOOL ShowMessage(char *lpszText, char *lpszCaption)
{
	::MessageBox(NULL, lpszText, lpszCaption, MB_OK);

	return TRUE;
}