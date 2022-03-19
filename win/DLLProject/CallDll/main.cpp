#include<Windows.h>

#pragma commect (lib,"Dll1")

extern "C" void MsgBox(WCHAR* szMsg);

int main()
{
	MsgBox(L"Hello First Dll!\n");
	return 0;
}