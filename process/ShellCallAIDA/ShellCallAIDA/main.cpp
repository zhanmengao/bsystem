#include"../../../Process/WindowsProcessHelper/Fork/ForkUtils.h"
#include<Shlwapi.h>
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#pragma comment(lib,"Shlwapi.lib")

#define PATH1 _T("aida64business\\aida64.exe")
#define PATH2 _T("aida64business\\AIDA64_v6.32.5600.exe")

int aide()
{

	//��ȡ��ǰ����Ŀ¼
	stdstr filedir = PathHelper::GetModuleFolder() + PATH2;
	printf("exe�ļ�·���� = %ws \n", filedir.c_str());
	if (PathFileExists(filedir.c_str()))//�ж��ļ��Ƿ���Ч TRUE ��Ч
	{
		{
			time_t begin = time(NULL);
			printf("��aida�ɼ����� ���ʱ���е㳤\n");
			// /R �������ڳ���	/ALL ��������	/SUM ��ϵͳժҪ	/HW	Ӳ��������� /SW ���������� /BENCH ���ܲ������� 
			//AUDIT �������� /CUSTOM �Զ���ѡ��
			//��aida�ɼ����� ���ʱ���е㳤����ȴ�����ɼ�������Ż�����ִ��
			stdstr report = PathHelper::GetModuleFolder() + _T("hw.ini");
			ForkUtils::ShellExec(filedir.c_str(),
				(_T("/HW /SILENT /INI /R ") + report),true);
			time_t end = time(NULL);
			printf("���ɳɹ� ... Path = %ws  %ld\n", report.c_str(), end - begin);
		}

		{
			time_t begin = time(NULL);
			printf("��aida�ɼ����� ���ʱ���е㳤\n");
			// /R �������ڳ���	/ALL ��������	/SUM ��ϵͳժҪ	/HW	Ӳ��������� /SW ���������� /BENCH ���ܲ������� 
			//AUDIT �������� /CUSTOM �Զ���ѡ��
			//��aida�ɼ����� ���ʱ���е㳤����ȴ�����ɼ�������Ż�����ִ��
			stdstr report = PathHelper::GetModuleFolder() + _T("sum.ini");
			ForkUtils::ShellExec(filedir.c_str(),(_T("/SUM /SILENT /INI /R ") + report),true);
			time_t end = time(NULL);
			// ʹ��SHGetSpecialFolderPath��ȡ����Ŀ¼·��
			printf("���ɳɹ� ... Path = %ws  %ld\n", report.c_str(), end - begin);
		}

		{
			time_t begin = time(NULL);
			printf("��aida�ɼ����� ���ʱ���е㳤\n");
			// /R �������ڳ���	/ALL ��������	/SUM ��ϵͳժҪ	/HW	Ӳ��������� /SW ���������� /BENCH ���ܲ������� 
			//AUDIT �������� /CUSTOM �Զ���ѡ��
			//��aida�ɼ����� ���ʱ���е㳤����ȴ�����ɼ�������Ż�����ִ��
			stdstr report = PathHelper::GetModuleFolder() + _T("sw.ini");
			ForkUtils::ShellExec(filedir.c_str(),
				(_T("/R /SW /SILENT /INI ") + report),true);
			time_t end = time(NULL);

			printf("���ɳɹ� ... Path = %ws  %ld\n", report.c_str(), end - begin);
		}
	}
	else
	{
		printf("�Ҳ��� aida64.exe �ļ� ����ŵ� %ws \n", filedir.c_str());
		return -1;
	}
	return 0;
}
void main()
{
	aide();
	system("pause");
}