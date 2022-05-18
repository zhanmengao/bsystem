#include"StartUpUtils.h"

void main()
{
	//StartUpUtils::StartUPAutoRun(PathHelper::GetModuleFilePath().c_str(), _T("QAQ123.exe"));
	std::vector<string> vec;
	StartUpUtils::GetStartUpList(vec);
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		cout << *it << endl;
	}
}