#include"RegeditTSApp.h"
#include"RegeditUtils.h"
#include<stdio.h>

void main()
{
	//autoRunTest();
	//RegeditMain();
	RegeditUtils::RegAutoRun_CurrentUser(_T("FileMonit"), _T("D:\\Space\\System\\Windows\\FileManager\\x64\\Debug\\FileManager.exe"));
	std::vector<stdstr> vec;
	RegeditUtils::ShowRunList(vec);
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		STDOUT << *it << std::endl;
	}
	system("pause");
}