#include"../../../Process/WindowsProcessHelper/Fork/ForkUtils.h"
#include<Shlwapi.h>
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#pragma comment(lib,"Shlwapi.lib")

#define PATH1 _T("aida64business\\aida64.exe")
#define PATH2 _T("aida64business\\AIDA64_v6.32.5600.exe")

int aide()
{

	//获取当前工作目录
	stdstr filedir = PathHelper::GetModuleFolder() + PATH2;
	printf("exe文件路径是 = %ws \n", filedir.c_str());
	if (PathFileExists(filedir.c_str()))//判断文件是否有效 TRUE 有效
	{
		{
			time_t begin = time(NULL);
			printf("打开aida采集数据 这个时间有点长\n");
			// /R 不弹窗口出来	/ALL 完整报告	/SUM 仅系统摘要	/HW	硬件相关内容 /SW 软件相关内容 /BENCH 性能测试内容 
			//AUDIT 监视内容 /CUSTOM 自定义选择
			//打开aida采集数据 这个时间有点长，会等待程序采集结束后才会向下执行
			stdstr report = PathHelper::GetModuleFolder() + _T("hw.ini");
			ForkUtils::ShellExec(filedir.c_str(),
				(_T("/HW /SILENT /INI /R ") + report),true);
			time_t end = time(NULL);
			printf("生成成功 ... Path = %ws  %ld\n", report.c_str(), end - begin);
		}

		{
			time_t begin = time(NULL);
			printf("打开aida采集数据 这个时间有点长\n");
			// /R 不弹窗口出来	/ALL 完整报告	/SUM 仅系统摘要	/HW	硬件相关内容 /SW 软件相关内容 /BENCH 性能测试内容 
			//AUDIT 监视内容 /CUSTOM 自定义选择
			//打开aida采集数据 这个时间有点长，会等待程序采集结束后才会向下执行
			stdstr report = PathHelper::GetModuleFolder() + _T("sum.ini");
			ForkUtils::ShellExec(filedir.c_str(),(_T("/SUM /SILENT /INI /R ") + report),true);
			time_t end = time(NULL);
			// 使用SHGetSpecialFolderPath获取特殊目录路径
			printf("生成成功 ... Path = %ws  %ld\n", report.c_str(), end - begin);
		}

		{
			time_t begin = time(NULL);
			printf("打开aida采集数据 这个时间有点长\n");
			// /R 不弹窗口出来	/ALL 完整报告	/SUM 仅系统摘要	/HW	硬件相关内容 /SW 软件相关内容 /BENCH 性能测试内容 
			//AUDIT 监视内容 /CUSTOM 自定义选择
			//打开aida采集数据 这个时间有点长，会等待程序采集结束后才会向下执行
			stdstr report = PathHelper::GetModuleFolder() + _T("sw.ini");
			ForkUtils::ShellExec(filedir.c_str(),
				(_T("/R /SW /SILENT /INI ") + report),true);
			time_t end = time(NULL);

			printf("生成成功 ... Path = %ws  %ld\n", report.c_str(), end - begin);
		}
	}
	else
	{
		printf("找不到 aida64.exe 文件 ，请放到 %ws \n", filedir.c_str());
		return -1;
	}
	return 0;
}
void main()
{
	aide();
	system("pause");
}