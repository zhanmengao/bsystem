#include"ForkUtils.h"
#include"ShellProcess.h"
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
int main()
{
	//BOOL bRet = ForkUtils::OpenURL(_T("http://www.baidu.com/"));

	ShellProcess sp;
	sp.SetPath(_T("powershell"), _T("D:\\Space\\System\\Process\\Install\\Install\\test.ps1"));
	sp.Run();
	sp.Wait();
	cout << sp.ExitCode() << endl;

	bool bRet = ForkUtils::ShellExec(PathHelper::GetModuleFolder() + _T("netdisk.exe"), _T(""));
	DWORD dwRet = GetLastError();
	printf("%d %d \n", bRet, dwRet);

	std::string waterexe = "Watermark.exe";
	std::string parameter;
	parameter.append(1 ? "1" : "0");
	parameter.append(" ");
	parameter.append(1 ? "1" : "0");
	parameter.append(" ");
	parameter += std::string("QAQ");
	parameter.append(" ");
	parameter += std::string("");
	parameter.append(" ");
	parameter += "9 ";
	parameter += "8 ";
	parameter += "7 ";
	parameter +=  "6 ";
	parameter += "5 ";
	parameter += "4";
	cout << parameter << endl;
	//if (ForkUtils::ShellExec(waterexe, parameter))
	getchar();
	return 0;
}