// ScreenCapture_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ScreenCapture.h"


int _tmain(int argc, _TCHAR* argv[])
{
	if (FALSE == ScreenCapture())
	{
		printf("Screen Cpature Error.\n");
	}

	printf("Screen Cpature OK.\n");
	system("pause");
	return 0;
}

