#pragma once
typedef struct _EUInfoStruct
{

	int CODE;
	int RERROR;
	WCHAR USERNAME[MAX_PATH];
	WCHAR PASSWORD[MAX_PATH];
	WCHAR ZPASSWORD[MAX_PATH];

	void zClear()
	{
		CODE = 0;
		RERROR = 0;
		ZeroMemory(USERNAME, sizeof(USERNAME));
		ZeroMemory(PASSWORD, sizeof(PASSWORD));
		ZeroMemory(ZPASSWORD, sizeof(ZPASSWORD));
	}

}EUInfoStruct;